#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct coordinate {
  int row, column;
};

void downShift(char grid[256][256], int fromRow, int gridHeight) {
  for (int r = gridHeight; r > fromRow; r--) {
    memcpy(grid[r], grid[r-1], gridHeight);
  }
}

void rightShift(char grid[256][256], int fromColumn, int gridWidth, int gridHeight) {
  for (int c = gridWidth; c > fromColumn; c--) {
    for (int r = 0; r < gridHeight; r++) grid[r][c] = grid[r][c-1];
  }
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

  char grid[256][256];
  int gridWidth = 0;
  int gridHeight;

  char line[256];
  int row = 0;
  while(fgets(line, 256, fp)) {
    if (!gridWidth) gridWidth = strlen(line) - 1;
    memcpy(grid[row++], line, gridWidth);
  }
  fclose(fp);
  gridHeight = gridWidth;

  // Expand rows
  for (int r = 0; r < gridHeight; r++) {
    int isEmpty = 1;
    for (int c = 0; c < gridWidth; c++) {
      if (grid[r][c] == '#') {
        isEmpty = 0;
        break;
      }
    }
    if (isEmpty) {
      downShift(grid, r++, gridHeight);
      gridHeight++;
    }
  }

  // Expand columns
  for (int c = 0; c < gridWidth; c++) {
    int isEmpty = 1;
    for (int r = 0; r < gridHeight; r++) {
      if (grid[r][c] == '#') {
        isEmpty = 0;
        break;
      }
    }
    if (isEmpty) {
      rightShift(grid, c++, gridWidth, gridHeight);
      gridWidth++;
    }
  }

  struct coordinate galaxies[1024];
  int galaxyCount = 0;

  for (int r = 0; r < gridHeight; r++) {
    for (int c = 0; c < gridWidth; c++) {
      if (grid[r][c] == '#') {
        galaxies[galaxyCount++] = (struct coordinate) {
          .row = r,
          .column = c
        };
      }
    }
  }

  int part1 = 0;
  for (int g1 = 0; g1 < galaxyCount; g1++) {
    for (int g2 = g1 + 1; g2 < galaxyCount; g2++) {
      struct coordinate loc1 = galaxies[g1];
      struct coordinate loc2 = galaxies[g2];
      part1 += abs(loc1.row - loc2.row) + abs(loc1.column - loc2.column);
    }
  }

  printf("Part 1: %d\n", part1);
  
  /*
  for (int r = 0; r < gridHeight; r++) {
    for (int c = 0; c < gridWidth; c++) {
      printf("%c", grid[r][c]);
    }
    printf("\n");
  }
  */


}
