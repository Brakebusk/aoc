#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARDSIZE 1000
#define DEBUG 0

void debug(int visited[BOARDSIZE][BOARDSIZE]) {
  for (int col = BOARDSIZE -1; col >= 0; col--) {
    for (int row = 0 ; row < BOARDSIZE; row++) {
        printf("%c", visited[row][col] ? '#' : '.');
    }
    printf("\n");
  }
  printf("\n");
}

void follow(int head[2], int tail[2], int visited[BOARDSIZE][BOARDSIZE], int mark) {
  if (abs(head[0] - tail[0]) <= 1 && abs(head[1] - tail[1]) <= 1) return;

  if (tail[0] == head[0] - 2) {
    tail[0]++;
    tail[1] = head[1];
  } else if (tail[0] == head[0] + 2) {
    tail[0]--;
    tail[1] = head[1];
  } else if (tail[1] == head[1] - 2) {
    tail[0] = head[0];
    tail[1]++;
  } else if (tail[1] == head[1] + 2) {
    tail[0] = head[0];
    tail[1]--;
  }

  if (mark) visited[tail[0]][tail[1]] = 1;
}

int simulate(char* filename, int knotCount) {
  FILE *fp = NULL;
  if ((fp = fopen(filename, "r")) == NULL) {
      printf("[ERROR] Failed to open file %s\n", filename);
      exit(EXIT_FAILURE);
  }

  int knots[knotCount][2];
  for (int i = 0; i < knotCount; i++) {
    knots[i][0] = BOARDSIZE / 2;
    knots[i][1] = BOARDSIZE / 2;
  }

  int visited[BOARDSIZE][BOARDSIZE];
  memset(visited, 0, BOARDSIZE * BOARDSIZE * sizeof(int));
  visited[BOARDSIZE / 2][BOARDSIZE / 2] = 1;

  char line[10];
  while(fgets(line, 10, fp)) {
    char direction;
    int magnitude;
    sscanf(line, "%c %d", &direction, &magnitude);

    switch (direction) {
      case 'U':
        for (int i = 0; i < magnitude; i++) {
          knots[0][1]++;
          for (int i = 0; i < knotCount - 1; i++) {
            follow(knots[i], knots[i + 1], visited, i == knotCount - 2);
          }
        }
        break;
      case 'R':
        for (int i = 0; i < magnitude; i++) {
          knots[0][0]++;
          for (int i = 0; i < knotCount - 1; i++) {
            follow(knots[i], knots[i + 1], visited, i == knotCount - 2);
          }
        }
        break;
      case 'D':
        for (int i = 0; i < magnitude; i++) {
          knots[0][1]--;
          for (int i = 0; i < knotCount - 1; i++) {
            follow(knots[i], knots[i + 1], visited, i == knotCount - 2);
          }
        }
        break;
      case 'L':
        for (int i = 0; i < magnitude; i++) {
          knots[0][0]--;
          for (int i = 0; i < knotCount - 1; i++) {
            follow(knots[i], knots[i + 1], visited, i == knotCount - 2);
          }
        }
        break;
      default:
        printf("[ERROR] Invalid direction '%c'\n", direction);
        exit(EXIT_FAILURE);
    }
  }

  int visitedCount = 0;
  for (int row = 0; row < BOARDSIZE; row++) {
    for (int col = 0; col < BOARDSIZE; col++) {
        visitedCount += visited[row][col];
    }
  }

  if (DEBUG) debug(visited);
  if (DEBUG) printf("Head: (%d, %d)\nTail: (%d, %d)\n", knots[0][0], knots[0][1], knots[knotCount - 1][0], knots[knotCount - 1][1]);

  fclose(fp);
  return visitedCount;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("[ERROR] Missing parameter <filename>\n");
    exit(EXIT_FAILURE);
  }

  char *filename = argv[1];

  printf("Part 1: %d\n", simulate(filename, 2));
  printf("Part 2: %d\n", simulate(filename, 10));
}