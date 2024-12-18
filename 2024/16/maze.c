#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_DEPTH 10000

struct point {
  int r, c;
};

struct point bestPaths[200][MAX_DEPTH];
int pathCount, bestPathLength = INT_MAX;
int startR, startC, endR, endC;

void traverse(char matrix[150][150], int distances[150][150], int size, int r, int c, int direction, int depth, struct point path[MAX_DEPTH]) {
  if (depth >= MAX_DEPTH) {
    printf("too deep!");
    exit(EXIT_FAILURE);
  }
  int distance = distances[r][c];

  path[depth].r = r;
  path[depth].c = c;

  if (r == endR && c == endC) {
    if (distance < bestPathLength) {
      bestPathLength = distance;
      pathCount = 0;
    }
    if (distance == bestPathLength) {
      memset(bestPaths[pathCount], 0, sizeof(struct point) * MAX_DEPTH);
      memcpy(bestPaths[pathCount++], path, sizeof(struct point) * depth);
    }
  }

  int deltas[4] = {1001, 1001, 1001, 1001};
  deltas[direction] = 1;

  if (matrix[r-1][c] != '#' && distances[r-1][c] + 1000 >= distance + deltas[0]) {
    distances[r-1][c] = distance + deltas[0];
    traverse(matrix, distances, size, r-1, c, 0, depth + 1, path);
  }
  if (matrix[r][c+1] != '#' && distances[r][c+1] + 1000 >= distance + deltas[1]) {
    distances[r][c+1] = distance + deltas[1];
    traverse(matrix, distances, size, r, c+1, 1, depth + 1, path);
  }
  if (matrix[r+1][c] != '#' && distances[r+1][c] + 1000 >= distance + deltas[2]) {
    distances[r+1][c] = distance + deltas[2];
    traverse(matrix, distances, size, r+1, c, 2, depth + 1, path);
  }
  if (matrix[r][c-1] != '#' && distances[r][c-1] + 1000 >= distance + deltas[3]) {
    distances[r][c-1] = distance + deltas[3];
    traverse(matrix, distances, size, r, c-1, 3, depth + 1, path);
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

  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      distances[r][c] = INT_MAX - MAX_DEPTH;
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

  struct point path[MAX_DEPTH];
  path[0].r = startR;
  path[1].c = startC;
  traverse(matrix, distances, size, startR, startC, 1, 1, path);
  printf("Part 1: %d\n", bestPathLength);

  int part2 = 1;
  for (int i = 0; i < pathCount; i++) {
    for (int p = 0; p < MAX_DEPTH; p++) {
      if (bestPaths[i][p].r > 0 && bestPaths[i][p].c > 0 && matrix[bestPaths[i][p].r][bestPaths[i][p].c] != 'O') {
        matrix[bestPaths[i][p].r][bestPaths[i][p].c] = 'O';
        part2++;
      }
    }
  }
  printf("Part 2: %d\n", part2);
}
