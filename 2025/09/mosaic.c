#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_SIZE 2000000000
size_t dimensions = 99999;
struct tile {
  int x, y;
};

int max(int a, int b) {
  return a > b ? a : b;
}

int min(int a, int b) {
  return a < b ? a : b;
}

long long getArea(struct tile a, struct tile b) {
  return (1 + llabs(a.x - b.x)) * (1 + llabs(a.y - b.y));
}

int validate(char (*grid)[99999], struct tile a, struct tile b) {
  int minX = min(a.x, b.x), 
      maxX = max(a.x, b.x), 
      minY = min(a.y, b.y), 
      maxY = max(a.y, b.y);

  for (int x = minX; x < maxX; x++) {
    for (int y = minY; y < maxY; y++) {
      if (!grid[x][y]) return 0;
    }
  }
  return 1;
}

void fill(char (*grid)[99999], int fromX, int fromY) {
  struct tile (*stack) = malloc(sizeof(struct tile) * STACK_SIZE);
  long long top = 0;
  stack[top++] = (struct tile){ fromX, fromY };
  grid[fromX][fromY] = 'X';

  long long counter = 0;
  long long maxTop = 0;
  while (top) {
    if (top > maxTop) maxTop = top;
    struct tile t = stack[--top];
    
    if (top >= STACK_SIZE - 4) {
      printf("Will exceed maximum stack size\n");
      exit(EXIT_FAILURE);
    }

    if (t.x > 0 && !grid[t.x-1][t.y]) {
      grid[t.x-1][t.y] = 'X';
      stack[top++] = (struct tile){ t.x-1, t.y };
    }
    if (t.x < 99998 && !grid[t.x+1][t.y]) {
      grid[t.x+1][t.y] = 'X';
      stack[top++] = (struct tile){ t.x+1, t.y };
    }
    if (t.y > 0 && !grid[t.x][t.y-1]) {
      grid[t.x][t.y-1] = 'X';
      stack[top++] = (struct tile){ t.x, t.y-1 };
    }
    if (t.y < 99998 && !grid[t.x][t.y+1]) {
      grid[t.x][t.y+1] = 'X';
      stack[top++] = (struct tile){ t.x, t.y+1 };
    }
  }

  free(stack);
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

  struct tile tiles[512];
  int tc = 0;

  char line[16];
  while(fgets(line, 16, fp)) {
    sscanf(line, "%d,%d", &tiles[tc].x, &tiles[tc].y);
    tc++;
  }
  fclose(fp);

  long long part1 = 0;

  for (int a = 0; a < tc; a++) {
    for (int b = a + 1; b < tc; b++) {
      long long area = getArea(tiles[a], tiles[b]);
      if (area > part1) part1 = area;
    }
  }

  printf("Part 1: %lld\n", part1);

  int fillX = 0, fillY = 0;

  char (*grid)[99999] = malloc(dimensions * dimensions);
  memset(grid, 0, dimensions * dimensions);
  for (int t = 0; t < tc; t++) {
    struct tile selected = tiles[t];
    struct tile next = t == tc - 1 ? tiles[0] : tiles[t + 1];
    
    grid[selected.x][selected.y] = '#';

    int x = selected.x;
    int y = selected.y;
    if (next.x > x) {
      while (++x < next.x) {
        if (!fillX) {
          fillX = x;
          fillY = y + 1;
        }
        grid[x][y] = 'X';
      }
    } else if (next.x < x) {
      while (--x > next.x) {
        grid[x][y] = 'X';
      }
    } else if (next.y > y) {
      while (++y < next.y) {
        grid[x][y] = 'X';
      }
    } else if (next.y < y) {
      while (--y > next.y) {
        grid[x][y] = 'X';
      }
    }
  }
  fill(grid, fillX, fillY);

  long long part2 = 0;

  for (int a = 0; a < tc; a++) {
    for (int b = a + 1; b < tc; b++) {
      if (validate(grid, tiles[a], tiles[b])) {
        long long area = getArea(tiles[a], tiles[b]);
        if (area > part2) part2 = area;
      }
    }
  }

  printf("Part 2: %lld\n", part2);
  free(grid);
}
