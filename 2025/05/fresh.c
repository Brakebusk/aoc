#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct range {
  long long min;
  long long max;
};

int compare(const void *a, const void* b) {
  struct range range1 = *(struct range*)a;
  struct range range2 = *(struct range*)b;

  if (range1.min > range2.min) {
    return 1;
  } else if (range1.min < range2.min) {
    return -1;
  } else return range1.max - range2.max;
}

int isWithinRange(struct range ranges[200], int rangeCount, long long check) {
  for (int r = 0; r < rangeCount; r++) {
    if (check >= ranges[r].min && check <= ranges[r].max) return 1;
  }
  return 0;
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

  int part1 = 0;

  struct range ranges[200];
  int rangeCount = 0;

  char line[64];
  int mode = 1;
  while(fgets(line, 64, fp)) {
    if (line[0] == '\n') {
      mode = 2;
      qsort(ranges, rangeCount, sizeof(struct range), compare);
      continue;
    }

    if (mode == 1) {
      sscanf(line, "%lld-%lld", &ranges[rangeCount].min, &ranges[rangeCount].max);
      rangeCount++;
    } else {
      long long check = atoll(line);
      part1 += isWithinRange(ranges, rangeCount, check);
    }
  }
  fclose(fp);

  printf("Part 1: %d\n", part1);

  long long part2 = 0;
  
  long long pointer = ranges[0].max;
  part2 += 1 + pointer - ranges[0].min;
  
  for (int r = 1; r < rangeCount; r++) {
    struct range selected = ranges[r];
    if (selected.min > pointer) pointer = selected.min;
    if (selected.max < pointer) continue;
    part2 += 1 + selected.max - pointer;
    pointer = 1 + selected.max;
  }

  printf("Part 2: %lld\n", part2);
}
