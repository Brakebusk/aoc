#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct machine {
  int adx, ady, bdx, bdy, x, y;
};

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

  struct machine machines[500];
  int count = 0;

  char line[32];
  int mode = 0;
  while(fgets(line, 32, fp)) {
    int i, j;
    switch (mode) {
      case 0:
        sscanf(line, "Button A: X+%d, Y+%d", &i, &j);
        machines[count].adx = i;
        machines[count].ady = j;
        break;
      case 1:
        sscanf(line, "Button B: X+%d, Y+%d", &i, &j);
        machines[count].bdx = i;
        machines[count].bdy = j;
        break;
      case 2:
        sscanf(line, "Prize: X=%d, Y=%d", &i, &j);
        machines[count].x = i;
        machines[count].y = j;
        break;
      case 3:
        count++;
    }
    mode = (mode + 1) % 4;
  }
  count++;
  fclose(fp);

  int part1 = 0;
  for (int i = 0; i < count; i++) {
    struct machine m = machines[i];
    int minTokens = INT_MAX;
    for (int a = 0; a < 101; a++) {
      for (int b = 0; b < 101; b++) {
        if (m.adx * a + m.bdx * b == m.x && m.ady * a + m.bdy * b == m.y) {
          int tokens = 3 * a + b;
          if (tokens < minTokens) minTokens = tokens;
        }
      }
    }
    if (minTokens < INT_MAX) part1 += minTokens;
  }

  printf("Part 1: %d\n", part1);
}
