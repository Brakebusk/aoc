#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check(char forward[4]) {
  char reverse[4];
  for (int i = 0; i < 4; i++) reverse[3-i] = forward[i];

  if (strncmp(forward, "XMAS", sizeof(char) * 4) == 0 ||
      strncmp(reverse, "XMAS", sizeof(char) * 4) == 0) return 1;
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

  char matrix[150][150];
  int size = 0;

  char line[150];
  int lc = 0;
  while(fgets(line, 150, fp)) {
    if (size == 0) size = strlen(line) - 1;
    memcpy(matrix[lc++], line, sizeof(char) * size);
  }
  fclose(fp);

  int part1 = 0;
  // Horizontal
  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size - 3 ; c++) {
      char buffer[4] ;
      for (int i = 0; i < 4; i++) buffer[i] = matrix[r][c+i];
      if (check(buffer)) part1++;
    }
  }

  // Vertical
  for (int c = 0; c < size ; c++) {
    for (int r = 0; r < size - 3; r++) {
      char buffer[4];
      for (int i = 0; i < 4; i++) buffer[i] = matrix[r+i][c];
      if (check(buffer)) part1++;
    }
  }

  // Diagonal down
  for (int r = 0; r < size; r++) {
    for (int d = 0; r+d < size - 3; d++) {
      char buffer[4];
      for (int i = 0; i < 4; i++) buffer[i] = matrix[r+d+i][d+i];
      if (check(buffer)) part1++;
    }
  }
  for (int c = 1; c < size; c++) {
    for (int d = 0; c+d < size - 3; d++) {
      char buffer[4];
      for (int i = 0; i < 4; i++) buffer[i] = matrix[d+i][c+d+i];
      if (check(buffer)) part1++;
    }
  }

  // Diagonal up
  for (int r = 3; r < size; r++) {
    for (int d = 0; r-d >= 3; d++) {
      char buffer[4];
      for (int i = 0; i < 4; i++) buffer[i] = matrix[r-d-i][d+i];
      if (check(buffer)) part1++;
    }
  }
  for (int c = 1; c < size - 3; c++) {
    for (int d = 0; c+d < size - 3; d++) {
      char buffer[4];
      for (int i = 0; i < 4; i++) buffer[i] = matrix[size-1- d-i][c+d+i];
      if (check(buffer)) part1++;
    }
  }

  printf("Part 1: %d\n", part1);
}
