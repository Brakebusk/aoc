#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int endR, endC;

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

void reset(int distances[150][150], int size, int startR, int startC) {
  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      distances[r][c] = INT_MAX;
    }
  }
  distances[startR][startC] = 0;
}

void testCheat(char matrix[150][150], int normalDistances[150][150], int timeSaved[10000], int size, int cheatR, int cheatC, int startR, int startC, int cheatLength) {
  if (matrix[cheatR][cheatC] == '.' && normalDistances[cheatR][cheatC] < INT_MAX) {
    for (int r = cheatR - cheatLength; r <= cheatR + cheatLength; r++) {
      for (int c = cheatC - cheatLength; c <= cheatC + cheatLength; c++) {
        if (r >= 0 && r < size && c >= 0 && c < size && matrix[r][c] == '.')  {
          int manhattan = abs(cheatR - r) + abs(cheatC - c);

          if (manhattan > 0 && manhattan <= cheatLength) {
            int cheatRemaining[150][150];
            reset(cheatRemaining, size, r, c);
            traverse(matrix, cheatRemaining, r, c, size);

            if (cheatRemaining[endR][endC] < INT_MAX) {
              int totalDistance = normalDistances[cheatR][cheatC] + manhattan + cheatRemaining[endR][endC];
              if (totalDistance < normalDistances[endR][endC]) timeSaved[normalDistances[endR][endC] - totalDistance]++;
            }
          }

        }
      }
    }
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

  int startR, startC;
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
        matrix[r][c] = '.';
        startR = r;
        startC = c;
      } else if (matrix[r][c] == 'E') {
        matrix[r][c] = '.';
        endR = r;
        endC = c;
      }
    }
  }

  int timeSaved[10000];
  memset(timeSaved, 0, sizeof(int) * 10000);
  int megaSaved[10000];
  memset(megaSaved, 0, sizeof(int) * 10000);

  reset(distances, size, startR, startC);
  traverse(matrix, distances, startR, startC, size);
  int benchmark = distances[endR][endC];

  for (int r = 1; r < size - 1; r++) {
    for (int c = 1; c < size - 1; c++) {
      testCheat(matrix, distances, timeSaved, size, r, c, startR, startC, 2);
      testCheat(matrix, distances, megaSaved, size, r, c, startR, startC, 20);
    }
  }

  int part1 = 0;
  int part2 = 0;
  for (int i = 100; i < 10000; i++) {
    part1 += timeSaved[i];
    part2 += megaSaved[i];
  }
  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}
