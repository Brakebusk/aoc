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
      printf("%c", pattern[row][col] ? '#' : '.');
    }
    printf("\n");
  }
}

int reverse(char bit) {
  return bit ? 0 : 1;
}

// Returns number of columns to the left of reflection point
int checkVerticalReflection(char pattern[32][32], int patternHeight, int patternWidth, int exclude) {
  for (int testColumn = 1; testColumn < patternWidth; testColumn++) {
    int testLength = min(testColumn, patternWidth - testColumn);
    int noReflection = 0;
    for (int row = 0; row < patternHeight; row++) {
      if (noReflection) break;
      for (int col = 0; col < testLength; col++) {
        if (pattern[row][testColumn - col - 1] != pattern[row][testColumn + col]) noReflection = 1;

      }
    }
    if (!noReflection && testColumn != exclude) return testColumn;
  }
  return 0;
}

// Returns number of rows above reflection point
int checkHorizontalReflection(char pattern[32][32], int patternHeight, int patternWidth, int exclude) {
  for (int testRow = 1; testRow < patternHeight; testRow++) {
    int testLength = min(testRow, patternHeight - testRow);
    int noReflection = 0;
    for (int col = 0; col < patternWidth; col++) {
      if (noReflection) break;
      for (int row = 0; row < testLength; row++) {
        if (pattern[testRow - row -1][col] != pattern[testRow + row][col]) noReflection = 1;

      }
    }
    if (!noReflection && testRow != exclude) return testRow;
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

  char pattern[32][32];
  int patternWidth = 0, patternHeight = 0;

  int part1 = 0;
  int part2 = 0;

  char line[512];
  while(fgets(line, 512, fp)) {
    int lineLength = strlen(line);
    if (lineLength == 1) {
      int columns = checkVerticalReflection(pattern, patternHeight, patternWidth, -1);
      int rows = 0;
      if (columns) {
        part1 += columns;
      } else {
        rows = checkHorizontalReflection(pattern, patternHeight, patternWidth, -1);
        part1 += 100 * rows;
      }

      int foundSmudge = 0;
      for (int row = 0; row < patternHeight; row++) {
        if (foundSmudge) break;
        for (int col = 0; col < patternWidth; col++) {
          pattern[row][col] = reverse(pattern[row][col]);
          int p2columns = checkVerticalReflection(pattern, patternHeight, patternWidth, columns);
          int p2rows = checkHorizontalReflection(pattern, patternHeight, patternWidth, rows);
          pattern[row][col] = reverse(pattern[row][col]);

          if (p2columns) {
            part2 += p2columns;
            foundSmudge = 1;
            break;
          } else if (p2rows) {
            part2 += 100 * p2rows;
            foundSmudge = 1;
            break;
          }
        }
      }

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
  printf("Part 2: %d\n", part2);
}
