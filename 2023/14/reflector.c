#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printGrid(char grid[128][128], int gridSize) {
  for (int row = 0; row < gridSize; row++) {
    for (int col = 0; col < gridSize; col++) {
      printf("%c", grid[row][col]);
    }
    printf("\n");
  }
  printf("\n");
}

int isRoundRock(char symbol) {
  return symbol == 'O';
}

int isCubedRock(char symbol) {
  return symbol == '#';
}

int isEmpty(char symbol) {
  return symbol == '.';
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

  char grid[128][128];
  int gridSize = 0;

  int row = 0;
  char line[128];
  while(fgets(line, 128, fp)) {
    if (!gridSize) gridSize = strlen(line) - 1;
    memcpy(grid[row++], line, gridSize);
  }
  fclose(fp);

  for (int row = 0; row < gridSize; row++) {
    for (int col = 0; col < gridSize; col++) {
      if (isRoundRock(grid[row][col])) {
        int localRow = row;
        while (localRow > 0 && isEmpty(grid[localRow-1][col])) {
          grid[localRow][col] = '.';
          grid[--localRow][col] = 'O';
        }
      }
    }
  }

  int part1 = 0;
  for (int row = 0; row < gridSize; row++) {
    for (int col = 0; col < gridSize; col++) {
      if (isRoundRock(grid[row][col])) part1 += gridSize-row;
    }
  }

  printf("Part 1: %d\n", part1);
}
