#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int min(int a, int b) {
  return a > b ? b : a;
}

int max(int a, int b) {
  return a > b ? a : b;
}

void printPattern(char pattern[32][32], int patternHeight, int patternWidth) {
  for (int row = 0; row < patternHeight; row++) {
    for (int col = 0; col < patternWidth; col++) {
      printf("%d", pattern[row][col]);
    }
    printf("\n");
  }
}

// Returns number of columns to the left of reflection point
int checkVerticalReflection(char pattern[32][32], int patternHeight, int patternWidth) {
  for (int testColumn = 1; testColumn < patternWidth; testColumn++) {
    int testLength = min(testColumn, patternWidth - testColumn);
    int noReflection = 0;
    for (int row = 0; row < patternHeight; row++) {
      if (noReflection) break;
      for (int col = 0; col < testLength; col++) {
        if (pattern[row][testColumn - col - 1] != pattern[row][testColumn + col]) noReflection = 1;

      }
    }
    if (!noReflection) return testColumn;
  }
  return 0;
}

// Returns number of rows above reflection point
int checkHorizontalReflection(char pattern[32][32], int patternHeight, int patternWidth) {
  for (int testRow = 1; testRow < patternHeight; testRow++) {
    int testLength = min(testRow, patternHeight - testRow);
    int noReflection = 0;
    for (int col = 0; col < patternWidth; col++) {
      if (noReflection) break;
      for (int row = 0; row < testLength; row++) {
        if (pattern[testRow - row -1][col] != pattern[testRow + row][col]) noReflection = 1;

      }
    }
    if (!noReflection) return testRow;
  }
  printf("No reflection point?!:\n");
  printPattern(pattern, patternHeight, patternWidth);
  exit(EXIT_FAILURE);
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

  char pattern[32][32];
  int patternWidth = 0, patternHeight = 0;

  int part1 = 0;

  char line[512];
  while(fgets(line, 512, fp)) {
    int lineLength = strlen(line);
    if (lineLength == 1) {
      int columns = checkVerticalReflection(pattern, patternHeight, patternWidth);
      if (columns) {
        part1 += columns;
      } else part1 += 100 * checkHorizontalReflection(pattern, patternHeight, patternWidth);

      patternHeight = 0;
      patternWidth = 0;
    } else {
      patternHeight++;
      if (!patternWidth) patternWidth = lineLength - 1;

      for (int c = 0; c < lineLength - 1; c++) {
        pattern[patternHeight - 1][c] = line[c] == '#' ? 1 : 0;
      }
    }
  }
  fclose(fp);

  printf("Part 1: %d\n", part1);
}
