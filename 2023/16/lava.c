#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tile {
  char symbol;
  char energized;
  int beamDirectionHistory[4];
};

struct beam {
  int row;
  int col;
  int direction;
};

int max(int a, int b) {
  return a > b ? a : b;
}

void printGrid (struct tile grid[128][128], int gridSize) {
  printf("Grid:\n");
  for (int row = 0; row < gridSize; row++) {
    for (int col = 0; col < gridSize; col++) {
      printf("%c", grid[row][col].energized ? '#' : grid[row][col].symbol);
    }
    printf("\n");
  }
  printf("\n");
};

int simulate(struct tile grid[128][128], int gridSize, int fromRow, int fromCol, int initialDirection) {
  struct beam beams[1024];
  int beamCount = 0;
    beams[beamCount++] = (struct beam) {
    .row = fromRow,
    .col = fromCol,
    .direction = initialDirection
  };

  for (int row = 0; row < gridSize; row++) {
    for (int col = 0; col < gridSize; col++) {
      grid[row][col].energized = 0;
      grid[row][col].beamDirectionHistory[0] = 0;
      grid[row][col].beamDirectionHistory[1] = 0;
      grid[row][col].beamDirectionHistory[2] = 0;
      grid[row][col].beamDirectionHistory[3] = 0;
    }
  }

  for (int beamIndex = 0; beamIndex < beamCount; beamIndex++) {
    struct beam currentBeam = beams[beamIndex];
    while (currentBeam.row >= 0 && currentBeam.row < gridSize && currentBeam.col >= 0 && currentBeam.col < gridSize) {
      int currentDirection = currentBeam.direction;
      if (currentDirection < 0 || currentDirection > 3) {
        printf("Invalid direction '%d'\n", currentDirection);
        exit(EXIT_FAILURE);
      }

      grid[currentBeam.row][currentBeam.col].energized = 1;
      if (grid[currentBeam.row][currentBeam.col].beamDirectionHistory[currentDirection]++) break;

      switch (grid[currentBeam.row][currentBeam.col].symbol) {
        case '.':
          if (currentDirection == 0) {
            currentBeam.row--;
          } else if (currentDirection == 1) {
            currentBeam.col++;
          } else if (currentDirection == 2) {
            currentBeam.row++;
          } else if (currentDirection == 3) {
            currentBeam.col--;
          }
          break;
        case '|': 
          if (currentDirection == 0) {
            currentBeam.row--;
          } else if (currentDirection == 1 || currentDirection == 3) {
            beams[beamCount++] = (struct beam) {
              .row = currentBeam.row + 1,
              .col = currentBeam.col,
              .direction = 2
            };
            currentBeam.row--;
            currentBeam.direction = 0;
          } else if (currentDirection == 2) {
            currentBeam.row++;
          }
          break;
        case '-':
          if (currentDirection == 0 || currentDirection == 2) {
            beams[beamCount++] = (struct beam) {
              .row = currentBeam.row,
              .col = currentBeam.col + 1,
              .direction = 1
            };
            currentBeam.col--;
            currentBeam.direction = 3;
          } else if (currentDirection == 1) {
            currentBeam.col++;
          } else if (currentDirection == 3) {
            currentBeam.col--;
          }
          break;
        case '\\':
          if (currentDirection == 0) {
            currentBeam.direction = 3;
            currentBeam.col--;
          } else if (currentDirection == 1) {
            currentBeam.direction = 2;
            currentBeam.row++;
          } else if (currentDirection == 2) {
            currentBeam.direction = 1;
            currentBeam.col++;
          } else if (currentDirection == 3) {
            currentBeam.direction = 0;
            currentBeam.row--;
          }
          break;
        case '/':
          if (currentDirection == 0) {
            currentBeam.direction = 1;
            currentBeam.col++;
          } else if (currentDirection == 1) {
            currentBeam.direction = 0;
            currentBeam.row--;
          } else if (currentDirection == 2) {
            currentBeam.direction = 3;
            currentBeam.col--;
          } else if (currentDirection == 3) {
            currentBeam.direction = 2;
            currentBeam.row++;
          }
          break;
        default:
          printf("Unknown tile symbol '%c'\n", grid[currentBeam.row][currentBeam.col].symbol);
          exit(EXIT_FAILURE);
      }
    };
  }

  int energized = 0;
  for (int row = 0; row < gridSize; row++) {
    for (int col = 0; col < gridSize; col++) {
      energized += grid[row][col].energized;
    }
  }
  return energized;
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

  struct tile grid[128][128];
  int gridSize = 0;

  char line[128];
  int lineCount = 0;
  while(fgets(line, 128, fp)) {
    if (!gridSize) gridSize = strlen(line) - 1;
    for (int col = 0; col < gridSize; col++) {
      grid[lineCount][col] = (struct tile) {
        .symbol = line[col], 
        .energized = 0,
        .beamDirectionHistory = {0}
      };
    }
    lineCount++;
  }
  fclose(fp);  
  printf("Part 1: %d\n", simulate(grid, gridSize, 0, 0, 1));

  int part2 = 0;
  for (int row = 0; row < gridSize; row++) {
    part2 = max(part2, simulate(grid, gridSize, row, 0, 1));
    part2 = max(part2, simulate(grid, gridSize, row, gridSize-1, 3));
  }
  for (int col = 0; col < gridSize; col++) {
    part2 = max(part2, simulate(grid, gridSize, 0, col, 2));
    part2 = max(part2, simulate(grid, gridSize, gridSize-1, col, 0));
  }
  printf("Part 2: %d\n", part2);
}
