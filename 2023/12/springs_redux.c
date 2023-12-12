#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

  for (int a = 0; a < arrCount; a++) {
    printf("Checking %d\n", a+1);
    struct arrangement arr = arrs[a];
    
    part1 += count(arr.springs, arr.springCount, arr.groups, arr.groupCount);
    
    int initSpringCount = arr.springCount;
    int initGroupCount = arr.groupCount;
    for (int i = 0; i < 4; i++) {
      arr.springs[arr.springCount++] = '?';
      memcpy(&arr.springs[arr.springCount], arr.springs, initSpringCount);
      arr.springCount += initSpringCount;
    
      memcpy(&arr.groups[arr.groupCount], arr.groups, sizeof(int) * initGroupCount);
      arr.groupCount += initGroupCount;
    }
    part2 += count(arr.springs, arr.springCount, arr.groups, arr.groupCount);
  }

  printf("Part 1: %lld\n", part1);
  printf("Part 2: %lld\n", part2);
}
