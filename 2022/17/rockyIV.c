#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 7
#define HEIGHT 4000

struct Cave {
  char cave[HEIGHT][WIDTH];
};

struct Cave* createCave() {
  struct Cave* cave = (struct Cave*) malloc(sizeof(struct Cave));
  memset(cave->cave, 0, WIDTH * HEIGHT * sizeof(char));
  return cave;
}

struct Jet {
  char direction[11000];
  int length;
  int counter;
};

struct Jet* createJet(char *line) {
  int length = strlen(line) - 1;
  struct Jet* jet = (struct Jet*) malloc(sizeof(struct Jet));
  jet->counter = 0;
  jet->length = length;
  for (int i = 0; i < length; i++) {
    jet->direction[i] = line[i];
  }
  return jet;
}

char getNextJet(struct Jet* jet) {
  return jet->direction[jet->counter++ % jet->length];
}

struct Shape {
  int width;
  int height;
  char definition[4][4];
};

struct Shape shapes[5] = {
  {4, 1, {
    {1, 1, 1, 1},
    {-1, -1, -1, -1},
    {-1, -1, -1, -1},
    {-1, -1, -1, -1},
  }},
    {3, 3, {
    {0, 1, 0, -1},
    {1, 1, 1, -1},
    {0, 1, 0, -1},
    {-1, -1, -1, -1},
  }},
    {3, 3, {
    {0, 0, 1, -1},
    {0, 0, 1, -1},
    {1, 1, 1, -1},
    {-1, -1, -1, -1},
  }},
    {1, 4, {
    {1, -1, -1, -1},
    {1, -1, -1, -1},
    {1, -1, -1, -1},
    {1, -1, -1, -1},
  }},
    {2, 2, {
    {1, 1, -1, -1},
    {1, 1, -1, -1},
    {-1, -1, -1, -1},
    {-1, -1, -1, -1},
  }}
};

int counter = 0;
struct Shape* getNextShape() {
  return &shapes[counter++ % 5];
}

void simulate(struct Cave* cave, struct Jet* jet) {
  return 0;
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
  
  char line[11000];
  fgets(line, 11000, fp);
  struct Jet* jet = createJet(line);

  struct Cave* cave = createCave();

  for (int i = 0; i < 2022; i++) simulate(cave, jet);

  for (int r = 0; r < HEIGHT; r++) {
    for (int c = 0; c < WIDTH; c++) {
      if (cave->cave[r][c] > 0) {
        printf("Part 1: %d\n", HEIGHT - r);
        break;
      }
    }
  }
  
  fclose(fp);
}