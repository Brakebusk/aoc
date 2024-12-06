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
    if (size == 0) size = strlen(line) - 1;
    memcpy(matrix[lc++], line, sizeof(char) * size);
  }
  fclose(fp);

  int r = -1, c = -1;
  int dir = 0; // 0 = up, 1 = right, 2 = down, 3 = left
  for (int rt = 0; rt < size && r == -1; rt++) {
    for (int ct = 0; ct < size; ct++) {
      if (matrix[rt][ct] == '^') {
        r = rt;
        c = ct;
        break;
      }
    }
  }

  while(1) {
    matrix[r][c] = 'X';
    if (dir == 0) {
      if (r > 0) {
        if (matrix[r-1][c] != '#') {
          r--;
        } else {
          dir++;
        }
      } else break;
    } else if (dir == 1) {
      if (c < size - 1) {
        if (matrix[r][c+1] != '#') {
          c++;
        } else {
          dir++;
        }
      } else break;
    } else if (dir == 2) {
      if (r < size - 1) {
        if (matrix[r+1][c] != '#') {
          r++;
        } else {
          dir++;
        }
      } else break;
    } else if (dir == 3) {
      if (c > 0) {
        if (matrix[r][c-1] != '#') {
          c--;
        } else {
          dir = 0;
        }
      } else break;
    }
  }

  int part1 = 0;
  for (int rt = 0; rt < size; rt++) {
    for (int ct = 0; ct < size; ct++) {
      if (matrix[rt][ct] == 'X') part1++;
    }
  }
  printf("Part 1: %d\n", part1);
}
