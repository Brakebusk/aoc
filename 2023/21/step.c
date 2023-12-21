#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIMENSIONS 64

struct tile {
  char symbol;
  char marked[DIMENSIONS];
};

void printGrid(struct tile grid[150][150], int gridSize) {
  for (int row = 0; row < gridSize; row++) {
    for (int col = 0; col < gridSize; col++) {
      struct tile t = grid[row][col];
      printf("%c", t.marked[0] ? 'O' : t.symbol);
    }
    printf("\n");
  }
  printf("\n");
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

  struct tile grid[150][150] = {0};
  int gridSize = 0;

  char line[150];
  int lineCounter = 0;
  while(fgets(line, 150, fp)) {
    if (!gridSize) gridSize = strlen(line) - 1;

    for (int c = 0; c < gridSize; c++) {
      int isStart = line[c] == 'S';
      grid[lineCounter][c].symbol = isStart ? '.' : line[c];
      grid[lineCounter][c].marked[0] = isStart;
    }

    lineCounter++;
  }
  fclose(fp);


  for (int s = 0; s < 64; s++) {
    struct tile newGrid[150][150];
    memcpy(newGrid, grid, sizeof(struct tile) * 150 * 150);
    
    for (int row = 0; row < gridSize; row++) {
      for (int col = 0; col < gridSize; col++) {
        for (int d = 0; d < DIMENSIONS; d++) {
          newGrid[row][col].marked[d] = 0;
        }
      }
    }

    for (int row = 0; row < gridSize; row++) {
      for (int col = 0; col < gridSize; col++) {
        if (grid[row][col].marked[0]) {
          if (grid[row+1][col].symbol == '.') {
            newGrid[row+1][col].marked[0] = 1;
          }
          if (grid[row-1][col].symbol == '.') {
            newGrid[row-1][col].marked[0] = 1;
          }
          if (grid[row][col+1].symbol == '.') {
            newGrid[row][col+1].marked[0] = 1;
          }
          if (grid[row][col-1].symbol == '.') {
            newGrid[row][col-1].marked[0] = 1;
          }
        }
      }
    }

    memcpy(grid, newGrid, sizeof(struct tile) * 150 * 150);
  }

  int part1 = 0;
  for (int row = 0; row < gridSize; row++) {
    for (int col = 0; col < gridSize; col++) {
      if (grid[row][col].marked[0]) part1++;
    }
  }

  printf("Part 1: %d\n", part1);
}
