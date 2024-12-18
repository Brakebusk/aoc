#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct claim {
  int id, col, row, width, height;
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

  int (*matrix)[1500] = malloc(sizeof(int[1500][1500]));
  memset(matrix, 0, sizeof(int) * 1500 * 1500);

  struct claim claims[1500];
  int claimCount = 0;

  char line[32];
  while(fgets(line, 32, fp)) {
    int id, col, row, width, height;
    sscanf(line, "#%d @ %d,%d: %dx%d", &id, &col, &row, &width, &height);

    claims[claimCount].id = id;
    claims[claimCount].col = col;
    claims[claimCount].row = row;
    claims[claimCount].width = width;
    claims[claimCount].height = height;
    claimCount++;

    for (int r = row; r < row+height; r++) {
      for (int c = col; c < col+width; c++) {
        matrix[r][c]++;
      }
    }
  }
  fclose(fp);

  int part1 = 0;
  for (int r = 0; r < 1500; r++) {
    for (int c = 0; c < 1500; c++) {
      if (matrix[r][c] > 1) part1++;
    }
  }
  printf("Part 1: %d\n", part1);

  for (int i = 0; i < claimCount; i++) {
    int id = claims[i].id;
    int row = claims[i].row;
    int col = claims[i].col;
    int height = claims[i].height;
    int width = claims[i].width;
    
    int valid = 1;
    for (int r = row; r < row+height && valid; r++) {
      for (int c = col; c < col+width; c++) {
        if (matrix[r][c] > 1) {
          valid = 0;
          break;
        }
      }
    }
    if (valid) {
      printf("Part 2: %d\n", id);
      break;
    }
  }
}
