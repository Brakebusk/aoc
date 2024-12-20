#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int startR, startC, endR, endC;

void traverse(char matrix[150][150], int distances[150][150], int r, int c, int size) {
  int distance = distances[r][c] + 1;

  if (r > 0 && matrix[r-1][c] != '#' && distances[r-1][c] > distance) {
    distances[r-1][c] = distance;
    traverse(matrix, distances, r-1, c, size);
  }
  if (c < size - 1 && matrix[r][c+1] != '#' && distances[r][c+1] > distance) {
    distances[r][c+1] = distance;
    traverse(matrix, distances, r, c+1, size);
  }
  if (r < size - 1 && matrix[r+1][c] != '#' && distances[r+1][c] > distance) {
    distances[r+1][c] = distance;
    traverse(matrix, distances, r+1, c, size);
  }
  if (c > 0 && matrix[r][c-1] != '#' && distances[r][c-1] > distance) {
    distances[r][c-1] = distance;
    traverse(matrix, distances, r, c-1, size);
  }
}

void reset(int distances[150][150], int size) {
  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      distances[r][c] = INT_MAX;
    }
  }
  distances[startR][startC] = 0;
}

int testCheat(char matrix[150][150], int distances[150][150], int size, int r, int c) {
  if (matrix[r][c] == '#') {
    matrix[r][c] = '.';
    reset(distances, size);
    traverse(matrix, distances, startR, startC, size);
    matrix[r][c] = '#';
    return distances[endR][endC];
  }
  return INT_MAX;
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

  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      if (matrix[r][c] == 'S') {
        startR = r;
        startC = c;
      } else if (matrix[r][c] == 'E') {
        endR = r;
        endC = c;
      }
    }
  }

  int timeSaved[10000];
  memset(timeSaved, 0, sizeof(int) * 10000);

  reset(distances, size);
  traverse(matrix, distances, startR, startC, size);
  int benchmark = distances[endR][endC];
  
  for (int r = 1; r < size - 1; r++) {
    for (int c = 1; c < size - 1; c++) {
      int test = testCheat(matrix, distances, size, r, c);
      if (test < benchmark) timeSaved[benchmark - test]++;
    }
  }

  int part1 = 0;
  for (int i = 100; i < 10000; i++) {
    part1 += timeSaved[i];
  }
  printf("Part 1: %d\n", part1);
}
