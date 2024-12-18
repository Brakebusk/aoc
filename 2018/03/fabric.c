#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

  char line[32];
  while(fgets(line, 32, fp)) {
    int id, col, row, width, height;
    sscanf(line, "#%d @ %d,%d: %dx%d", &id, &col, &row, &width, &height);

    printf("Got %d %d %d %d %d\n", id, col, row, width, height);

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
}
