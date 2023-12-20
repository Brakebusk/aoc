#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define GRID_SIZE 16000
#define MID_POINT 8000

struct point {
  int visit[2];
};

int min(int a, int b) {
  return a > b ? b : a;
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

  struct point (*grid)[GRID_SIZE] = malloc(sizeof(struct point[GRID_SIZE][GRID_SIZE]));
  memset(grid, 0, sizeof(struct point[GRID_SIZE][GRID_SIZE]));

  char line[2048];
  int wireIndex = 0;
  while(fgets(line, 2048, fp)) {
    int steps = 0;
    int row = MID_POINT;
    int col = MID_POINT;

    char *token = 0;
    while ((token = strtok(token ? NULL : line, ","))) {
      char dir = token[0];
      int magnitude = atoi(&token[1]);

      switch (dir) {
        case 'U':
          for (int i = 0; i < magnitude; i++) {
            row++;
            grid[row][col].visit[wireIndex] = ++steps;
          }
          break;
        case 'R':
          for (int i = 0; i < magnitude; i++) {
            col++;
            grid[row][col].visit[wireIndex] = ++steps;
          }
          break;
        case 'D':
          for (int i = 0; i < magnitude; i++) {
            row--;
            grid[row][col].visit[wireIndex] = ++steps;
          }
          break;
        case 'L':
          for (int i = 0; i < magnitude; i++) {
            col--;
            grid[row][col].visit[wireIndex] = ++steps;
          }
          break;
        default:
          printf("Unknown direction %c\n", dir);
          exit(EXIT_FAILURE);
      }

      if (row < 0 || row >= GRID_SIZE || col < 0 || col >= GRID_SIZE) {
        printf("Invalid position (%d, %d)\n", row, col);
        exit(EXIT_FAILURE);
      }
    }
    wireIndex++;
  }
  fclose(fp);

  int part1 = INT_MAX;
  int part2 = INT_MAX;

  for (int row = 0; row < GRID_SIZE; row++) {
    for (int col = 0; col < GRID_SIZE; col++) {
      if (grid[row][col].visit[0] && grid[row][col].visit[1]) {
        part1 = min(part1, abs(MID_POINT - row) + abs(MID_POINT - col));
        part2 = min(part2, grid[row][col].visit[0] + grid[row][col].visit[1]);
      }
    }
  }

  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
  free(grid);
}
