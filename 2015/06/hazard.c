#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int max(int a, int b) {
  return a > b ? a : b;
}

int min(int a, int b) {
  return a < b ? a : b;
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

  char grid[1000][1000] = {0};

  char line[64];
  while(fgets(line, 64, fp)) {
    char tokens[5][16] = {0};
    int tokenCount = 0;

    char *token = 0;
    while ((token = strtok(token ? NULL : line, " "))) {
      strcpy(tokens[tokenCount++], token);
    }

    int mode;
    int coordinateIndex;
    if (tokenCount == 5) {
      coordinateIndex = 2;
      if (strcmp(tokens[1], "on") == 0) {
        mode = 1;
      } else {
        mode = 0;
      }
    } else {
      coordinateIndex = 1;
      mode = 2;
    }

    int a,b,c,d;
    token = strtok(tokens[coordinateIndex], ",");
    a = atoi(token);
    token = strtok(NULL, ",");
    b = atoi(token);
    token = strtok(tokens[coordinateIndex+2], ",");
    c = atoi(token);
    token = strtok(NULL, ",");
    d = atoi(token);

    for (int row = min(a, c); row <= max(a, c); row++) {
      for (int col = min(b, d); col <= max(b, d); col++) {
        if (mode == 2) {
          grid[row][col] = !grid[row][col];
        } else {
          grid[row][col] = mode;
        }
      }
    }
  }
  fclose(fp);

  int part1 = 0;
  for (int row = 0; row < 1000; row++) {
    for (int col = 0; col < 1000; col++) {
      part1 += grid[row][col];
    }
  }
  printf("Part 1: %d\n", part1);
}
