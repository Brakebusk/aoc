#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARDSIZE 1000
#define DEBUG 0
#define LENGTH 10

void debug(int visited[2][BOARDSIZE][BOARDSIZE]) {
  for (int col = BOARDSIZE -1; col >= 0; col--) {
    for (int row = 0 ; row < BOARDSIZE; row++) {
        printf("%c", visited[1][row][col] ? '#' : '.');
    }
    printf("\n");
  }
  printf("\n");
}

void follow(int head[2], int tail[2], int visited[2][BOARDSIZE][BOARDSIZE], int mark) {
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

  if (mark >= 0) visited[mark][tail[0]][tail[1]] = 1;
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

  int knots[LENGTH][2];
  for (int i = 0; i < LENGTH; i++) {
    knots[i][0] = BOARDSIZE / 2;
    knots[i][1] = BOARDSIZE / 2;
  }

  int visited[2][BOARDSIZE][BOARDSIZE];
  memset(visited, 0, 2 * BOARDSIZE * BOARDSIZE * sizeof(int));
  visited[0][BOARDSIZE / 2][BOARDSIZE / 2] = 1;
  visited[1][BOARDSIZE / 2][BOARDSIZE / 2] = 1;

  char line[10];
  while(fgets(line, 10, fp)) {
    char direction;
    int magnitude;
    sscanf(line, "%c %d", &direction, &magnitude);

    switch (direction) {
      case 'U':
        for (int i = 0; i < magnitude; i++) {
          knots[0][1]++;
          for (int j = 0; j < LENGTH - 1; j++) {
            follow(knots[j], knots[j + 1], visited, !j ? 0 : j == LENGTH - 2 ? 1 : -1);
          }
        }
        break;
      case 'R':
        for (int i = 0; i < magnitude; i++) {
          knots[0][0]++;
          for (int j = 0; j < LENGTH - 1; j++) {
            follow(knots[j], knots[j + 1], visited, !j ? 0 : j == LENGTH - 2 ? 1 : -1);
          }
        }
        break;
      case 'D':
        for (int i = 0; i < magnitude; i++) {
          knots[0][1]--;
          for (int j = 0; j < LENGTH - 1; j++) {
            follow(knots[j], knots[j + 1], visited, !j ? 0 : j == LENGTH - 2 ? 1 : -1);
          }
        }
        break;
      case 'L':
        for (int i = 0; i < magnitude; i++) {
          knots[0][0]--;
          for (int j = 0; j < LENGTH - 1; j++) {
            follow(knots[j], knots[j + 1], visited, !j ? 0 : j == LENGTH - 2 ? 1 : -1);
          }
        }
        break;
      default:
        printf("[ERROR] Invalid direction '%c'\n", direction);
        exit(EXIT_FAILURE);
    }
  }

  int part1 = 0;
  int part2 = 0;
  for (int row = 0; row < BOARDSIZE; row++) {
    for (int col = 0; col < BOARDSIZE; col++) {
        part1 += visited[0][row][col];
        part2 += visited[1][row][col];
    }
  }

  if (DEBUG) debug(visited);
  if (DEBUG) printf("Head: (%d, %d)\nTail: (%d, %d)\n", knots[0][0], knots[0][1], knots[9][0], knots[9][1]);

  fclose(fp);

  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}