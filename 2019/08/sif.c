#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct layer {
  int grid[6][25];
};

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

  int width, height;

  char line[16000];
  fgets(line, 16000, fp);
  sscanf(line, "%dx%d", &width, &height);
  fgets(line, 16000, fp);
  fclose(fp);

  struct layer layers[1024];
  int layerCount = strlen(line) / (width * height);

  int cIndex = 0;
  for (int l = 0; l < layerCount; l++) {
    for (int row = 0; row < height; row++) {
      for (int col = 0; col < width; col++) {
        layers[l].grid[row][col] = line[cIndex++] - '0';
      }
    }
  }

  int fewestZeroes = INT_MAX;
  int part1 = 0;
  for (int l = 0; l < layerCount; l++) {
    int zeroes = 0;
    int ones = 0;
    int twos = 0;
    for (int row = 0; row < height; row++) {
      for (int col = 0; col < width; col++) {
        if (layers[l].grid[row][col] == 0) zeroes++;
        if (layers[l].grid[row][col] == 1) ones++;
        if (layers[l].grid[row][col] == 2) twos++;
      }
    }
    if (zeroes < fewestZeroes) {
      fewestZeroes = zeroes;
      part1 = ones * twos;
    }
  }

  printf("Part 1: %d\n\nPart2:\n", part1);

  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      for (int l = 0; l < layerCount; l++) {
        int v = layers[l].grid[row][col];
        if (v < 2) {
          printf("%c", v == 0 ? ' ' : '#');
          break;
        }
      }
    }
    printf("\n");
  }
}
