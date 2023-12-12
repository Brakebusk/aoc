#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct arrangement {
  char springs[32];
  int springCount;
  int groups[16];
  int groupCount;
};

int check(char springs[32], int springCount, int groups[16], int groupCount) {
  int trueGroups[32] = {0};
  int trueGroupLength = 1;

  for (int s = 0; s < springCount; s++) {
    switch (springs[s]) {
      case '.':
        if (trueGroups[trueGroupLength-1]) {
          trueGroupLength++;
        }
        break;
      case '#':
        trueGroups[trueGroupLength-1]++;
        break;
      default:
        printf("Invalid spring '%c'\n", springs[s]);
        exit(EXIT_FAILURE);
    }
  }

  if (!trueGroups[trueGroupLength-1]) trueGroupLength--;

  if (trueGroupLength != groupCount) return 0;

  for (int g = 0; g < trueGroupLength; g++) {
    if (trueGroups[g] != groups[g]) return 0;
  }
  return 1;
}

int countUnknown(struct arrangement arr) {
  int unknown = 0;
  for (int s = 0; s < arr.springCount; s++) {
    if (arr.springs[s] == '?') unknown++;
  }
  return unknown;
}

// Index 0 = LSB
int getBitValue(int num, int index) {
    return (num & (1 << index)) != 0;
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

  for (int a = 0; a < arrCount; a++) {
    struct arrangement arr = arrs[a];
    printf("\nSpring states (%d):\n", arr.springCount);
    for (int c = 0; c < arr.springCount; c++) printf("%c", arr.springs[c]);
    printf("\nGroups:\n");
    for (int g = 0; g < arr.groupCount; g++) printf("%d ", arr.groups[g]);
    printf("\n");
  }
  printf("\n");

  int part1 = 0;

  for (int a = 0; a < arrCount; a++) {
    struct arrangement arr = arrs[a];
    int unknowns = countUnknown(arr);
    int combinations = 1 << unknowns;

    for (int cand = 0; cand < combinations; cand++) {
      char springCandidate[32];

      int unknownIndex = 0;
      for (int s = 0; s < arr.springCount; s++) {
        if (arr.springs[s] == '?') {
          springCandidate[s] = getBitValue(cand, unknownIndex++) ? '#' : '.';
        } else {
          springCandidate[s] = arr.springs[s];
        }
      }

      if(check(springCandidate, arr.springCount, arr.groups, arr.groupCount)) part1++;
    }
  }

  printf("Part 1: %d\n", part1);
}
