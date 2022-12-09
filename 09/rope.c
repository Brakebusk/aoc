#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARDSIZE 500
#define START 249

void debug(int head[2], int tail[2], int visited[BOARDSIZE][BOARDSIZE]) {
  for (int col = BOARDSIZE -1; col >= 0; col--) {
    for (int row = 0 ; row < BOARDSIZE; row++) {
        printf("%c", visited[row][col] ? '#' : '.');
    }
    printf("\n");
  }
  printf("\n");
}

void follow(int head[2], int tail[2], int visited[BOARDSIZE][BOARDSIZE]) {
  //printf("Head: (%d, %d) Tail: (%d, %d)\n", head[0], head[1], tail[0], tail[1]);
  if (abs(head[0] - tail[0]) <= 1 && abs(head[1] - tail[1]) <= 1) return;

  //printf("Following\n");

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

  //printf("Head: (%d, %d) Tail: (%d, %d)\n", head[0], head[1], tail[0], tail[1]);

  visited[tail[0]][tail[1]] = 1;
  //debug(head, tail, visited);
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

  int head[2] = {START, START};
  int tail[2] = {START, START};
  int visited[BOARDSIZE][BOARDSIZE];
  memset(visited, 0, BOARDSIZE * BOARDSIZE * sizeof(int));
  visited[START][START] = 1;

  char line[10];
  while(fgets(line, 10, fp)) {
    char direction;
    int magnitude;
    sscanf(line, "%c %d", &direction, &magnitude);

    switch (direction) {
      case 'U':
        for (int i = 0; i < magnitude; i++) {
          //printf("Up\n");
          head[1]++;
          follow(head, tail, visited);
        }
        break;
      case 'R':
        for (int i = 0; i < magnitude; i++) {
          //printf("Right\n");
          head[0]++;
          follow(head, tail, visited);
        }
        break;
      case 'D':
        for (int i = 0; i < magnitude; i++) {
          //printf("Down\n");
          head[1]--;
          follow(head, tail, visited);
        }
        break;
      case 'L':
        for (int i = 0; i < magnitude; i++) {
          //printf("Left\n");
          head[0]--;
          follow(head, tail, visited);
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

  debug(head, tail, visited);
  printf("Head: (%d, %d)\nTail: (%d, %d)\n", head[0], head[1], tail[0], tail[1]);

  printf("Part 1: %d\n", visitedCount);

  fclose(fp);
}