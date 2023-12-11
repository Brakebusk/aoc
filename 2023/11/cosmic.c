#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PART_2_FACTOR 1000000 - 1

struct coordinate {
  int row, column;
};

int max(int a, int b) {
  return a > b ? a : b;
}

int min(int a, int b) {
  return a < b ? a : b;
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
  int gridSize = 0;

  char line[256];
  int row = 0;
  while(fgets(line, 256, fp)) {
    if (!gridSize) gridSize = strlen(line) - 1;
    memcpy(grid[row++], line, gridSize);
  }
  fclose(fp);

  struct coordinate galaxies[1024];
  int galaxyCount = 0;

  int emptyRows[1024];
  int emptyRowCount = 0;

  for (int r = 0; r < gridSize; r++) {
    int isEmpty = 1;
    for (int c = 0; c < gridSize; c++) {
      if (grid[r][c] == '#') {
        isEmpty = 0;
        galaxies[galaxyCount++] = (struct coordinate) {
          .row = r,
          .column = c
        };
      }
    }
    if (isEmpty) {
      emptyRows[emptyRowCount++] = r;
    }
  }

  int emptyColumns[1024];
  int emptyColumnCount = 0;

  for (int c = 0; c < gridSize; c++) {
    int isEmpty = 1;
    for (int r = 0; r < gridSize; r++) {
      if (grid[r][c] == '#') {
        isEmpty = 0;
        break;
      }
    }
    if (isEmpty) {
      emptyColumns[emptyColumnCount++] = c;
    }
  }

  int part1 = 0;
  long long part2 = 0;

  for (int g1 = 0; g1 < galaxyCount; g1++) {
    for (int g2 = g1 + 1; g2 < galaxyCount; g2++) {
      struct coordinate loc1 = galaxies[g1];
      struct coordinate loc2 = galaxies[g2];
      int taxiCab = abs(loc1.row - loc2.row) + abs(loc1.column - loc2.column);
      part1 += taxiCab;
      part2 += taxiCab;

      for (int eR = 0; eR < emptyRowCount; eR++) {
        int row = emptyRows[eR];
        if (max(loc1.row, loc2.row) > row && min(loc1.row, loc2.row) < row) {
          part1 += 1;
          part2 += PART_2_FACTOR;
        }
      }
      for (int eC = 0; eC < emptyColumnCount; eC++) {
        int column = emptyColumns[eC];
        if (max(loc1.column, loc2.column) > column && min(loc1.column, loc2.column) < column) {
          part1 += 1;
          part2 += PART_2_FACTOR;
        }
      }
    }
  }

  printf("Part 1: %d\n", part1);
  printf("Part 2: %lld\n", part2);
}
