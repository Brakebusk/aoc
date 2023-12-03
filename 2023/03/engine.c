#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MATRIX_SIZE = 140;

void printMatrix(char matrix[MATRIX_SIZE][MATRIX_SIZE]) {
  printf("Matrix:\n");
  for (int i = 0; i < MATRIX_SIZE; i++) {
    for (int j = 0; j < MATRIX_SIZE; j++) {
      printf("%c", matrix[i][j]);
    }
    printf("\n");
  }
}

int isSymbol(char c) {
  return (c >= '!' && c <= '-') || (c >= ':' && c <= '@') || c == '/';
}

int max(int a, int b) {
  return a > b ? a : b;
}

int min(int a, int b) {
  return a < b ? a : b;
}

struct number {
  int value,
      startX,
      endX,
      y;
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

  char matrix[MATRIX_SIZE][MATRIX_SIZE];
  memset(matrix, 0, sizeof(matrix));

  char line[256];
  int row = 0;
  while(fgets(line, 256, fp)) {
    memcpy(matrix[row++], line, MATRIX_SIZE);
  }
  fclose(fp);

  struct number numbers[2048];
  int numbersCount = 0;
  for (int row = 0; row < MATRIX_SIZE; row++) {
    for (int col = 0; col < MATRIX_SIZE; col++) {
      if (matrix[row][col] >= '0' && matrix[row][col] <= '9') {
        int value = matrix[row][col] - '0';
        int startX = col;
        int endX = col;
        int y = row;

        while (matrix[row][++col] >= '0' && matrix[row][col] <= '9' && col < MATRIX_SIZE) {
          value = value * 10 + (matrix[row][col] - '0');
          endX = col;
        }

        numbers[numbersCount++] = (struct number) {
          .value = value,
          .startX = startX,
          .endX = endX,
          .y = y
        };
      }
    }
  }

  int part1 = 0;
  int part2 = 0;

  for (int i = 0; i < numbersCount; i++) {
    int value = numbers[i].value;
    int startX = numbers[i].startX;
    int endX = numbers[i].endX;
    int y = numbers[i].y;

    int foundAdjacentSymbol = 0;

    // Check left
    if (startX > 0 && isSymbol(matrix[y][startX - 1])) {
      foundAdjacentSymbol = 1;
    }
    // Check right
    if (endX < MATRIX_SIZE - 1 && isSymbol(matrix[y][endX + 1])) {
      foundAdjacentSymbol = 1;
    }
    if (y > 0) {
      for (int x = max(startX - 1, 0); x <= min(endX + 1, MATRIX_SIZE-1); x++) {
        if (isSymbol(matrix[y - 1][x])) {
          foundAdjacentSymbol = 1;
          break;
        }
      }
    }
    if (y < MATRIX_SIZE - 1) {
      for (int x = max(startX - 1, 0); x <= min(endX + 1, MATRIX_SIZE-1); x++) {
        if (isSymbol(matrix[y + 1][x])) {
          foundAdjacentSymbol = 1;
          break;
        }
      }
    }

    if (foundAdjacentSymbol) {
      part1 += value;
    }
  }

    for (int row = 0; row < MATRIX_SIZE; row++) {
      for (int col = 0; col < MATRIX_SIZE; col++) {
        char c = matrix[row][col];
        if (c == '*') {
          printf("\nFound star at %d, %d\n", row, col);
          struct number adjacentNumbers[10];
          int adjacentNumbersCount = 0;
          for (int i = 0; i < numbersCount; i++) {
            int value = numbers[i].value;
            int startX = numbers[i].startX;
            int endX = numbers[i].endX;
            int y = numbers[i].y;

            if (abs(y - row) <= 1) {
              if (abs(startX - col) <= 1 || abs(endX - col) <= 1) {
                adjacentNumbers[adjacentNumbersCount++] = numbers[i];
              }
            }
          }

          if (adjacentNumbersCount == 2) {
            part2 += adjacentNumbers[0].value * adjacentNumbers[1].value;
          }
        }
      }
    }

  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}

