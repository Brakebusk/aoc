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
        while ((token = strtok(NULL, " "))) {
          seeds[seedCount++] = strtoll(token, NULL, 10);
        }

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
      if (maps[mapCount-1].length > 0) {
        for (int s = 0; s < seedCount; s++) {
          for (int m = 0; m < maps[mapCount-1].length; m++) {
            long long selectedSeed = seeds[s];
            struct map selectedMap = maps[mapCount-1].maps[m];
            if (selectedSeed >= selectedMap.sourceRangeStart && selectedSeed <= selectedMap.sourceRangeStart + selectedMap.rangeLength) {
              seeds[s] = selectedMap.destinationRangeStart + (selectedSeed - selectedMap.sourceRangeStart);
              break;
            }
          }
        }
      }
      mapCount++;
      skip = 1;
    }
  }
  fclose(fp);

  long long part1 = __LONG_LONG_MAX__;
  for (int s = 0; s < seedCount; s++) {
    if (part1 > seeds[s]) part1 = seeds[s];
  }
  printf("Part 1: %lld\n", part1);
}

