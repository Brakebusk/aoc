#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int traverse(char matrix[150][150], int size) {
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

  int history[150*150][3];
  int hLength = 0;

  while(1) {
    matrix[r][c] = 'X';

    for (int h = 0; h < hLength; h++) {
      if (history[h][0] == r && history[h][1] == c && history[h][2] == dir) return 0;
    }

    history[hLength][0] = r;
    history[hLength][1] = c;
    history[hLength][2] = dir;
    hLength++;
    if (hLength >= 150*150) {
      printf("[ERROR] Exceeded history buffer\n");
      exit(EXIT_FAILURE);
    }

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

  return 1;
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

  char masterMatrix[150][150];
  int size = 0;

  char line[150];
  int lc = 0;
  while(fgets(line, 150, fp)) {
    if (size == 0) size = strlen(line) - 1;
    memcpy(masterMatrix[lc++], line, sizeof(char) * size);
  }
  fclose(fp);

  char scratchMatrix[150][150];

  int part1 = 0;
  for (int r = 0; r < size; r++) memcpy(scratchMatrix[r], masterMatrix[r], sizeof(char) * size);
  traverse(scratchMatrix, size);
  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      if (scratchMatrix[r][c] == 'X') part1++;
    }
  }
  printf("Part 1: %d\n", part1);

  int part2 = 0;
  for (int r = 0; r < size; r++) {
    printf("Testing r %d\n", r);
    for (int c = 0; c < size; c++) {
      if (masterMatrix[r][c] == '.') {
        for (int rc = 0; rc < size; rc++) memcpy(scratchMatrix[rc], masterMatrix[rc], sizeof(char) * size);
        scratchMatrix[r][c] = '#';
        if (traverse(scratchMatrix, size) == 0) part2++;
      }
    }
  }
  printf("Part 2: %d\n", part2);
}
