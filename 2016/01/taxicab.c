#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int mod(int a, int b) {
  return (a % b + b) % b;
}

int turn(int dir, int delta) {
  return mod(dir + delta, 4);
}

int distance(int x, int y) {
  return abs(x) + abs(y);
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

  char line[1024];
  fgets(line, 1024, fp);
  fclose(fp);

  char visited[1024][1024] = {0};
  visited[0][0] = 1;

  int part2 = 0;

  int x = 0, y = 0, dir = 0;
  char *token = 0;
  while ((token = strtok(token ? NULL : line, ", "))) {
    dir = turn(dir, token[0] == 'R' ? 1 : -1);
    int magnitude = atoi(&token[1]);

    switch (dir) {
      case 0:
        while (magnitude--) {
          y += 1;
          if (visited[x][y] && !part2) part2 = distance(x, y);
          visited[x][y] = 1;
        }
        break;
      case 1:
        while (magnitude--) {
          x += 1;
          if (visited[x][y] && !part2) part2 = distance(x, y);
          visited[x][y] = 1;
        }
        break;
      case 2:
        while (magnitude--) {
          y -= 1;
          if (visited[x][y] && !part2) part2 = distance(x, y);
          visited[x][y] = 1;
        }
        break;
      case 3:
        while (magnitude--) {
          x -= 1;
          if (visited[x][y] && !part2) part2 = distance(x, y);
          visited[x][y] = 1;
        }
        break;
      default:
        printf("Invalid direction: %d\n", dir);
        exit(EXIT_FAILURE);
    }
  }

  printf("Part 1: %d\n", distance(x, y));
  printf("Part 2: %d\n", part2);
}
