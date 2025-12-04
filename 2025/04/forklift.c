#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int min(int a, int b) {
  return a > b ? b : a;
}

int max(int a, int b) {
  return a > b ? a : b;
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

  char matrix[150][150];

  char line[150];
  int size = 0;
  int lc = 0;
  while(fgets(line, 150, fp)) {
    if (!size) size = strlen(line) - 1;
    memcpy(matrix[lc++], line, size);
  }
  fclose(fp);

  int part1 = 0;

  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      if (matrix[r][c] == '@') {
        int neighbours = 0;
        for (int cr = max(r - 1, 0); cr <= min(r + 1, size - 1); cr++) {
          for (int cc = max(c - 1, 0); cc <= min(c + 1, size - 1); cc++) {
            if (cr != r || cc != c) {
              if (matrix[cr][cc] == '@') neighbours++;
            }
          }
        }
        if (neighbours < 4) part1++;
      }
    }
  }

  printf("Part 1: %d\n", part1);
}
