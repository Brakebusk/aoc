#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char matrix[50][50];
int size = 0;

// from top = 0, right, down, left
int traverse(int r, int c, int from) {
  char v = matrix[r][c];
  if (v == 9) {
    matrix[r][c] = -2;
    return 1;
  }
  int sum = 0;
  if (from != 0 && r > 0 && matrix[r-1][c] == v+1) sum += traverse(r-1, c, 2);
  if (from != 1 && c < size-1 && matrix[r][c+1] == v+1) sum += traverse(r, c+1, 3);
  if (from != 2 && r < size-1 && matrix[r+1][c] == v+1) sum += traverse(r+1, c, 0);
  if (from != 3 && c > 0 && matrix[r][c-1] == v+1) sum += traverse(r, c-1, 1);
  return sum;
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

  char line[50];
  int lc = 0;
  while(fgets(line, 50, fp)) {
    if (size == 0) size = strlen(line) - 1;
    memcpy(matrix[lc++], line, sizeof(char) * size);
  }
  fclose(fp);

  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      matrix[r][c] = matrix[r][c] == '.' ? -1 : matrix[r][c] - '0';
    }
  }

  int part1 = 0;
  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      if (matrix[r][c] == 0) {
        part1 += traverse(r, c, -1);
        for (int cr = 0; cr < size; cr++) {
          for (int cc = 0; cc < size; cc++) {
            if (matrix[cr][cc] == -2) matrix[cr][cc] = 9;
          }
        }
      }
    }
  }
  printf("Part 1: %d\n", part1);
}
