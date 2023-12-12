#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

struct arrangement {
  char springs[160];
  int springCount;
  int groups[80];
  int groupCount;
};

long long count(char springs[], int springCount, int groups[], int groupCount) {
  if (springCount == 0) return groupCount == 0 ? 1 : 0;

  if (groupCount == 0) {
    for (int s = 0; s < springCount; s++) {
      if (springs[s] == '#') return 0;
    }
    return 1;
  }

  long long result = 0;

  if (springs[0] == '.' || springs[0] == '?') {
    result += count(&springs[1], springCount - 1, groups, groupCount);
  }

  if (springs[0] == '#' || springs[0] == '?') {
    if (groups[0] <= springCount) {
      int hasDot = 0;
      for (int s = 0; s < groups[0]; s++) {
        if (springs[s] == '.') {
          hasDot = 1;
          break;
        }
      }

      if (!hasDot && (groups[0] == springCount || springs[groups[0]] != '#')) {
        result += count(&springs[groups[0] + 1], springCount - groups[0] - 1, &groups[1], groupCount - 1);
      }
    }
  }

  return result;
}

struct arg_struct {
  int *finished;
  struct arrangement *arr;
};

void *thread(void *arguments) {
  struct arg_struct *args = (struct arg_struct *)arguments;

  struct arrangement *arr = (struct arrangement *)args->arr;
  int initSpringCount = arr->springCount;
  int initGroupCount = arr->groupCount;
  for (int i = 0; i < 4; i++) {
    arr->springs[arr->springCount++] = '?';
    memcpy(&arr->springs[arr->springCount], arr->springs, initSpringCount);
    arr->springCount += initSpringCount;
  
    memcpy(&arr->groups[arr->groupCount], arr->groups, sizeof(int) * initGroupCount);
    arr->groupCount += initGroupCount;
  }

  long long res = count(arr->springs, arr->springCount, arr->groups, arr->groupCount);
  printf("Finish #%d\n", (*args->finished)++);
  return (void *) res;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("[ERROR] Missing parameter <filename>\n");
    exit(EXIT_FAILURE);
  }

  char *filename = argv[1];

  FILE *fp = NULL;
  if ((fp = fopen(filename, "r")) == NULL) {
      printf("[ERROR] Failed to open file %s\n", filename);
      exit(EXIT_FAILURE);
  }

  struct arrangement arrs[1024];
  int arrCount = 0;

  char line[64];
  while(fgets(line, 64, fp)) {
    char *token = strtok(line, " ");

    int springCount = strlen(token);
    memcpy(arrs[arrCount].springs, token, springCount);
    arrs[arrCount].springCount = springCount;

    arrs[arrCount].groupCount = 0;
    while ((token = strtok(NULL, ","))) {
      arrs[arrCount].groups[arrs[arrCount].groupCount++] = atoi(token);
    }

    arrCount++;
  }
  fclose(fp);

  long long part1 = 0;
  long long part2 = 0;

  pthread_t threads[1024];
  struct arg_struct args[1024];
  long long results[1024] = {0};
  int threadCount = 0;
  int finished = 0;

  for (int a = 0; a < arrCount; a++) {
    part1 += count(arrs[a].springs, arrs[a].springCount, arrs[a].groups, arrs[a].groupCount);
    
    args[threadCount] = (struct arg_struct) {
      .arr = &arrs[a],
      .finished = &finished
    };
    pthread_create(&threads[threadCount], NULL, thread, (void *) &args[threadCount]);
    threadCount++;
  }

  for (int i = 0; i < threadCount; i++) {
    pthread_join(threads[i], (void *) &results[i]);
  }

  for (int i = 0; i < threadCount; i++) {
    part2 += results[i];
  }

  printf("Part 1: %lld\n", part1);
  printf("Part 2: %lld\n", part2);
}
