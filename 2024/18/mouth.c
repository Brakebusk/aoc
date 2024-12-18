#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define SIZE 71
#define LIMIT 1024

void traverse(char matrix[SIZE][SIZE], int distances[SIZE][SIZE], int r, int c) {
  int distance = distances[r][c] + 1;

  if (r > 0 && matrix[r-1][c] != '#' && distances[r-1][c] > distance) {
    distances[r-1][c] = distance;
    traverse(matrix, distances, r-1, c);
  }
  if (c < SIZE - 1 && matrix[r][c+1] != '#' && distances[r][c+1] > distance) {
    distances[r][c+1] = distance;
    traverse(matrix, distances, r, c+1);
  }
  if (r < SIZE - 1 && matrix[r+1][c] != '#' && distances[r+1][c] > distance) {
    distances[r+1][c] = distance;
    traverse(matrix, distances, r+1, c);
  }
  if (c > 0 && matrix[r][c-1] != '#' && distances[r][c-1] > distance) {
    distances[r][c-1] = distance;
    traverse(matrix, distances, r, c-1);
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

  char matrix[SIZE][SIZE];
  memset(matrix, '.', SIZE * SIZE);
  int distances[SIZE][SIZE];
  for (int r = 0; r < SIZE; r++) {
    for (int c = 0; c < SIZE; c++) {
      distances[r][c] = INT_MAX;
    }
  }
  distances[0][0] = 0;

  char line[8];
  int lc = 0;
  while(fgets(line, 8, fp) && lc++ < LIMIT) {
    int a, b;
    sscanf(line, "%d,%d", &a, &b);
    matrix[b][a] = '#';
  }
  fclose(fp);

  /*
  for (int r = 0; r < SIZE; r++) {
    for (int c = 0; c < SIZE; c++) {
      printf("%c", matrix[r][c]);
    }
    printf("\n");
  }
  printf("\n");
  */
  traverse(matrix, distances, 0, 0);
  printf("Part 1: %d\n", distances[SIZE-1][SIZE-1]);
}
