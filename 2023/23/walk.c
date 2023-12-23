#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int max(int a, int b) {
  return a > b ? a : b;
}

int min(int a, int b) {
  return a < b ? a : b;
}

struct tile {
  char symbol;
  char marked;
};

int traverse(struct tile grid[150][150], int gridSize, int row, int col, int distance, int goal[2]) {
  if (row == goal[0] && col == goal[1]) return distance;

  grid[row][col].marked = 1;

  char currentSymbol = grid[row][col].symbol;

  int maxDist = -1;
  if (currentSymbol == '.' || currentSymbol == '^' && row > 0) {
    if (!grid[row-1][col].marked && grid[row-1][col].symbol != '#') maxDist = max(maxDist, traverse(grid, gridSize, row-1, col, distance+1, goal));
  }
  if (currentSymbol == '.' || currentSymbol == '>' && col + 1 < gridSize) {
    if (!grid[row][col+1].marked && grid[row][col+1].symbol != '#') maxDist = max(maxDist, traverse(grid, gridSize, row, col+1, distance+1, goal));
  }
  if (currentSymbol == '.' || currentSymbol == 'v' && row + 1 < gridSize) {
    if (!grid[row+1][col].marked && grid[row+1][col].symbol != '#') maxDist = max(maxDist, traverse(grid, gridSize, row+1, col, distance+1, goal));
  }
  if (currentSymbol == '.' || currentSymbol == '<' && col > 0) {
    if (!grid[row][col-1].marked && grid[row][col-1].symbol != '#') maxDist = max(maxDist, traverse(grid, gridSize, row, col-1, distance+1, goal));
  }

  grid[row][col].marked = 0;
  return maxDist;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("[ERROR] Missing parameter <filename>\n");
    exit(EXIT_FAILURE);
  }

  char *filename = argv[1];

  struct tile grid[150][150];
  int gridSize = 0;

  FILE *fp = NULL;
  if ((fp = fopen(filename, "r")) == NULL) {
      printf("[ERROR] Failed to open file %s\n", filename);
      exit(EXIT_FAILURE);
  }

  int start[2], end[2];

  int lineCounter = 0;
  char line[150];
  while(fgets(line, 150, fp)) {
    if (!gridSize) gridSize = strlen(line) - 1;
    for (int c = 0; c < gridSize; c++) {
      grid[lineCounter][c] = (struct tile) {
        .symbol = line[c],
        .marked = 0
      };
      if (lineCounter == 0 && line[c] == '.') {
        start[0] = lineCounter;
        start[1] = c;
      } else if (lineCounter == gridSize - 1 && line[c] == '.') {
        end[0] = lineCounter;
        end[1] = c;
      }
    }
    lineCounter++;
  }
  fclose(fp);

  printf("Part 1: %d\n", traverse(grid, gridSize, start[0], start[1], 0, end));
}
