#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

  // Part 2 seed routing
  long long part2 = __LONG_LONG_MAX__;
  for (int sRang = 0; sRang < seedRangeCount; sRang++) {
    printf("Checking seed range %d of %d\n", sRang+1, seedRangeCount);
    long long rangeStart = seedRanges[sRang].rangeStart;
    long long rangeLength = seedRanges[sRang].rangeLength;
    for (long long seed = rangeStart; seed < rangeStart + rangeLength; seed++) {
      long long currentValue = seed;

      for (int mSet = 0; mSet < mapCount; mSet++) {
        for (int m = 0; m < maps[mSet].length; m++) {
          struct map selectedMap = maps[mSet].maps[m];
          if (currentValue >= selectedMap.sourceRangeStart && currentValue < selectedMap.sourceRangeStart + selectedMap.rangeLength) {
            currentValue = selectedMap.destinationRangeStart + (currentValue - selectedMap.sourceRangeStart);
            break;
          }
        }
      }

      if (currentValue < part2) part2 = currentValue;
    }
  }
  printf("Part 1: %lld\n", part1);
  printf("Part 2: %lld\n", part2);
}

