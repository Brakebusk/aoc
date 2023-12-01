#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 7
#define HEIGHT 4000

struct Cave {
  char cave[HEIGHT][WIDTH];
  int currentHeight;
};

struct Cave* createCave() {
  struct Cave* cave = (struct Cave*) malloc(sizeof(struct Cave));
  memset(cave->cave, 0, WIDTH * HEIGHT * sizeof(char));
  cave->currentHeight = 0;
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

int checkCollision(struct Cave *cave, struct Shape *shape, int x, int y, char direction) {
  switch(direction) {
    case '>':
      x++;
      break;
    case '<':
      y--;
      break;
    default:
      printf("Sanity: '%c'\n", direction);
  }

  for (int r = y; r < y + shape->height; r++) {
    for (int c = x; c < x + shape->width; c++) {
      if ()
    }
  }
}

void simulate(struct Cave* cave, struct Jet* jet) {
  struct Shape *nextShape = getNextShape();
  int x = cave->currentHeight + 3;
  int y = 2;

  while(1) {

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
  
  char line[11000];
  fgets(line, 11000, fp);
  struct Jet* jet = createJet(line);

  struct Cave* cave = createCave();

  for (int i = 0; i < 2022; i++) simulate(cave, jet);

  printf("Part 1: %d\n", cave->currentHeight);
  
  fclose(fp);
}