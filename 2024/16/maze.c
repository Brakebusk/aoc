#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

void traverse(char matrix[150][150], int distances[150][150], int size, int r, int c, int direction) {
  int distance = distances[r][c];

  int deltas[4] = {1001, 1001, 1001, 1001};
  deltas[direction] = 1;

  if (matrix[r-1][c] != '#' && distances[r-1][c] > distance + deltas[0]) {
    distances[r-1][c] = distance + deltas[0];
    traverse(matrix, distances, size, r-1, c, 0);
  }
  if (matrix[r][c+1] != '#' && distances[r][c+1] > distance + deltas[1]) {
    distances[r][c+1] = distance + deltas[1];
    traverse(matrix, distances, size, r, c+1, 1);
  }
  if (matrix[r+1][c] != '#' && distances[r+1][c] > distance + deltas[2]) {
    distances[r+1][c] = distance + deltas[2];
    traverse(matrix, distances, size, r+1, c, 2);
  }
  if (matrix[r][c-1] != '#' && distances[r][c-1] > distance + deltas[3]) {
    distances[r][c-1] = distance + deltas[3];
    traverse(matrix, distances, size, r, c-1, 3);
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

  char matrix[150][150];
  int distances[150][150];
  int size = 0;

  char line[150];
  int lc = 0;
  while(fgets(line, 150, fp)) {
    if (size == 0) size = strlen(line) - 1;
    memcpy(matrix[lc++], line, size);
  }
  fclose(fp);

  int startR, startC, endR, endC;
  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      distances[r][c] = INT_MAX;
      if (matrix[r][c] == 'S') {
        startR = r;
        startC = c;
        distances[r][c] = 0;
      } else if (matrix[r][c] == 'E') {
        endR = r;
        endC = c;
      }
    }
  }

  traverse(matrix, distances, size, startR, startC, 1);
  printf("Part 1: %d\n", distances[endR][endC]);
}
