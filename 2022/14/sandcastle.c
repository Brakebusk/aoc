#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 600
#define HEIGHT 200

enum Element {
  Source = -1,
  Air = 0,
  Rock = 1,
  Sand = 2,
};

struct Slice {
  int sourceX;
  int sourceY;
  char grid[HEIGHT][WIDTH];
};

void set(struct Slice* slice, int x, int y, char value) {
  slice->grid[y][x] = value;
}

char get(struct Slice* slice, int x, int y) {
  return slice->grid[y][x];
}

struct Slice* createSlice(int sourceX, int sourceY) {
  struct Slice* slice = (struct Slice*) malloc(sizeof(struct Slice));
  memset(slice, 0, sizeof(struct Slice));
  slice->sourceX = sourceX;
  slice->sourceY = sourceY;
  set(slice, sourceX, sourceY, Source);
  return slice;
}

void parseLine(struct Slice* slice, char* line) {
  int brushX = -1, brushY = -1;

  char *token;
  token = strtok(line, "->");
  while (token != NULL) {
    int x, y;
    sscanf(token, "%d,%d", &x, &y);

    if (brushX < 0) {
      brushX = x;
      brushY = y;
      set(slice, x, y, Rock);
    } else {
      int xDir = x > brushX ? 1 : -1;
      int yDir = y > brushY ? 1 : -1;
      while (brushX != x) {
        set(slice, brushX += xDir, y, Rock);
      }
      while (brushY != y) {
        set(slice, x, brushY += yDir, Rock);
      }
    }

    token = strtok(NULL, "->");
  }
}

int simulate(struct Slice* slice) {
  if (get(slice, slice->sourceX, slice->sourceY+1) != Air) return 0;

  int sandX = slice->sourceX, sandY = slice->sourceY;

  while(1) {
    if (sandY == HEIGHT - 1) {
      return 0;
    } else if (get(slice, sandX, sandY + 1) == Air) {
      sandY++;
    } else if (get(slice, sandX - 1, sandY + 1) == Air) {
      sandX--;
      sandY++;
    } else if (get(slice, sandX + 1, sandY + 1) == Air) {
      sandX++;
      sandY++;
    } else {
      set(slice, sandX, sandY, Sand);
      break;
    }
  }
  return 1;
}

void debug(struct Slice* slice, int round, int horizontalOffset, int length) {
  printf("After round %d:\n", round);
  for (int r = 0; r < HEIGHT; r++) {
    for (int c = horizontalOffset; c < WIDTH && c <= (horizontalOffset + length); c++) {
      switch(get(slice, c, r)) {
        case Source:
          printf("+");
          break;
        case Air:
          printf(".");
          break;
        case Rock:
          printf("#");
          break;
        case Sand:
          printf("o");
          break;
        default:
          printf("[SANITY] Invalid material %d\n", get(slice, c, r));
          exit(EXIT_FAILURE);
          break;
      }
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

  struct Slice* slice = createSlice(500, 0);
  
  char line[400];
  while(fgets(line, 400, fp)) {
    parseLine(slice, line);
  }

  debug(slice, 0, 490, 20);

  int round = 0;
  while (simulate(slice) != 0) {
    round++;
  }
  
  fclose(fp);

  printf("Part 1: %d\n", round);
}