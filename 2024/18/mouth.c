#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define SIZE 71
#define LIMIT 1024

void reset(int distances[SIZE][SIZE]) {
  for (int r = 0; r < SIZE; r++) {
    for (int c = 0; c < SIZE; c++) {
      distances[r][c] = INT_MAX;
    }
  }
  distances[0][0] = 0;
}

int traverse(char matrix[SIZE][SIZE], int distances[SIZE][SIZE], int r, int c, int skipEarly) {
  int distance = distances[r][c] + 1;

  if (skipEarly && r == SIZE - 1 && c == SIZE - 1) return 1;

  if (r > 0 && matrix[r-1][c] != '#' && distances[r-1][c] > distance) {
    distances[r-1][c] = distance;
    if (traverse(matrix, distances, r-1, c, skipEarly)) return 1;
  }
  if (c < SIZE - 1 && matrix[r][c+1] != '#' && distances[r][c+1] > distance) {
    distances[r][c+1] = distance;
    if (traverse(matrix, distances, r, c+1, skipEarly)) return 1;
  }
  if (r < SIZE - 1 && matrix[r+1][c] != '#' && distances[r+1][c] > distance) {
    distances[r+1][c] = distance;
    if (traverse(matrix, distances, r+1, c, skipEarly)) return 1;
  }
  if (c > 0 && matrix[r][c-1] != '#' && distances[r][c-1] > distance) {
    distances[r][c-1] = distance;
    if (traverse(matrix, distances, r, c-1, skipEarly)) return 1;
  }

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

  char matrix[SIZE][SIZE];
  memset(matrix, '.', SIZE * SIZE);
  int distances[SIZE][SIZE];
  reset(distances);

  int bytes[3500][2];

  char line[8];
  int lc = 0;
  while(fgets(line, 8, fp)) {
    int a, b;
    sscanf(line, "%d,%d", &a, &b);
    bytes[lc][0] = b;
    bytes[lc++][1] = a;
  }
  fclose(fp);

  int byteCounter;
  for (int byteCounter = 0; byteCounter < LIMIT; byteCounter++) {
    matrix[bytes[byteCounter][0]][bytes[byteCounter][1]] = '#';
  }
  traverse(matrix, distances, 0, 0, 0);
  printf("Part 1: %d\n", distances[SIZE-1][SIZE-1]);

  byteCounter = LIMIT;
  do {
    reset(distances);
    int r = bytes[byteCounter][0];
    int c = bytes[byteCounter][1];
    matrix[r][c] = '#';
    traverse(matrix, distances, 0, 0, 1);
    if (distances[SIZE-1][SIZE-1] == INT_MAX) {
      printf("Part 2: %d,%d\n", c, r);
      break;
    }
  } while (byteCounter++ < lc);
}
