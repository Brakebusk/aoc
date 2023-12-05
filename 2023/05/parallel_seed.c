#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<pthread.h>

#define MAX_THREAD_COUNT 128
#define THREADS_PER_SEED_RANGE 8

struct map {
  long long destinationRangeStart, 
            sourceRangeStart,
            rangeLength;
};

struct mapArray {
  int length;
  struct map maps[50];
};

struct seedRange {
  long long rangeStart,
            rangeLength;
};

struct arg_struct {
  int id;
  long long rangeStart;
  long long rangeLength;
  int mapCount;
  struct mapArray (*maps)[8];
};

void *thread(void *arguments) {
  struct arg_struct *args = (struct arg_struct *)arguments;

  int id = args->id;
  printf("Thread %d starting..\n", id);
  long long rangeStart = args->rangeStart;
  long long rangeLength = args->rangeLength;
  int mapCount = args->mapCount;
  struct mapArray (* maps)[8] = args->maps;
  long long min = __LONG_LONG_MAX__;
  
  for (long long seed = rangeStart; seed < rangeStart + rangeLength; seed++) {
    long long currentValue = seed;

    for (int mSet = 0; mSet < mapCount; mSet++) {
      for (int m = 0; m < (* maps)[mSet].length; m++) {
        struct map selectedMap = (* maps)[mSet].maps[m];
        if (currentValue >= selectedMap.sourceRangeStart && currentValue < selectedMap.sourceRangeStart + selectedMap.rangeLength) {
          currentValue = selectedMap.destinationRangeStart + (currentValue - selectedMap.sourceRangeStart);
          break;
        }
      }
    }

    if (currentValue < min) min = currentValue;
  }

  printf("Thread %d finished (%lld)..\n", id, min);

  return (void *) min;
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

  long long seeds[32];
  int seedCount = 0;

  struct seedRange seedRanges[16];
  int seedRangeCount = 0;

  struct mapArray maps[8];
  for (int i = 0; i < 8; i++) maps[i] = (struct mapArray) {
    .length = 0
  };
  int mapCount = 0;

  char line[256];
  int skip = 0;
  int mode = 1;
  while(fgets(line, 256, fp)) {
    if (skip) {
      skip = 0;
    } else if (strlen(line) > 1) {
      if (mode == 1) {
        char *token;
        token = strtok(line, ":");
        // Add part 1 seeds
        while ((token = strtok(NULL, " "))) {
          seeds[seedCount++] = strtoll(token, NULL, 10);
        }

        // Add part 2 seed ranges
        for (int i = 0; i < seedCount; i += 2) {
          seedRanges[seedRangeCount++] = (struct seedRange) {
            .rangeStart = seeds[i],
            .rangeLength = seeds[i+1]
          };
        }

        mode++;
      } else {
        long long dest, source, length;
        sscanf(line, "%lld %lld %lld", &dest, &source, &length);
        maps[mapCount-1].maps[maps[mapCount-1].length++] = (struct map) {
          .destinationRangeStart = dest,
          .sourceRangeStart = source,
          .rangeLength = length
        };
      }
    } else {
      mapCount++;
      skip = 1;
    }
  }
  fclose(fp);

  // Part 1 seed routing
  for (int mSet = 0; mSet < mapCount; mSet++) {
    for (int s = 0; s < seedCount; s++) {
      for (int m = 0; m < maps[mSet].length; m++) {
        long long selectedSeed = seeds[s];
        struct map selectedMap = maps[mSet].maps[m];
        if (selectedSeed >= selectedMap.sourceRangeStart && selectedSeed < selectedMap.sourceRangeStart + selectedMap.rangeLength) {
          seeds[s] = selectedMap.destinationRangeStart + (selectedSeed - selectedMap.sourceRangeStart);
          break;
        }
      }
    }
  }

  long long part1 = __LONG_LONG_MAX__;
  for (int s = 0; s < seedCount; s++) {
    if (part1 > seeds[s]) part1 = seeds[s];
  }

  pthread_t threads[MAX_THREAD_COUNT];
  struct arg_struct args[MAX_THREAD_COUNT];
  int threadCount = 0;

  // Part 2 seed routing
  long long part2 = __LONG_LONG_MAX__;
  for (int sRang = 0; sRang < seedRangeCount; sRang++) {
    long long rangeLength = seedRanges[sRang].rangeLength;
    long long rangeStart = seedRanges[sRang].rangeStart;
    long long stepLength = rangeLength / THREADS_PER_SEED_RANGE;
    for (int i = 0; i < THREADS_PER_SEED_RANGE; i++) {
      args[threadCount] = (struct arg_struct) {
        .id = threadCount,
        .rangeStart = rangeStart + i * stepLength,
        .rangeLength = stepLength,
        .mapCount = mapCount,
        .maps = &maps
      };
      pthread_create(&threads[threadCount], NULL, thread, (void *) &args[threadCount]);
      threadCount++;
    }
  }

  printf("All threads created\n");

  long long results[MAX_THREAD_COUNT] = {0};
  for (int i = 0; i < threadCount; i++) {
    pthread_join(threads[i], (void *) &results[i]);
  }

  for (int i = 0; i < threadCount; i++) {
    if (part2 > results[i]) part2 = results[i];
  }

  printf("Part 1: %lld\n", part1);
  printf("Part 2: %lld\n", part2);
}

