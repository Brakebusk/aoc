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

  char matrix[150][150];
  int size = 0;

  char line[150];
  int lc = 0;
  while(fgets(line, 150, fp)) {
    if (!size) size = strlen(line) - 1;
    memcpy(matrix[lc++], line, size);
  }
  fclose(fp);

  for (int c = 0; c < size; c++) {
    if (matrix[0][c] == 'S') {
      matrix[1][c] = '|';
      break;
    }
  }

  int part1 = 0;

  for (int r = 1; r < size - 1; r++) {
    for (int c = 0; c < size; c++) {
      if (matrix[r][c] == '|') {
        if (matrix[r+1][c] == '.') {
          matrix[r+1][c] = '|';
        } else if (matrix[r+1][c] == '^') {
          matrix[r+1][c+1] = '|';
          matrix[r+1][c-1] = '|';
          part1++;
        }
      }
    }
  }

  printf("Part 1: %d\n", part1);
}
