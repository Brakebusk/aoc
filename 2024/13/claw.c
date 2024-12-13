#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct machine {
  long long adx, ady, bdx, bdy, x, y;
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
    long long i, j;
    switch (mode) {
      case 0:
        sscanf(line, "Button A: X+%lld, Y+%lld", &i, &j);
        machines[count].adx = i;
        machines[count].ady = j;
        break;
      case 1:
        sscanf(line, "Button B: X+%lld, Y+%lld", &i, &j);
        machines[count].bdx = i;
        machines[count].bdy = j;
        break;
      case 2:
        sscanf(line, "Prize: X=%lld, Y=%lld", &i, &j);
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

  long long part1 = 0;
  long long part2 = 0;
  for (int i = 0; i < count; i++) {
    struct machine m = machines[i];
    long long minTokens = LLONG_MAX;
    for (int a = 0; a < 101; a++) {
      for (int b = 0; b < 101; b++) {
        if (m.adx * a + m.bdx * b == m.x && m.ady * a + m.bdy * b == m.y) {
          long long tokens = 3 * a + b;
          if (tokens < minTokens) minTokens = tokens;
        }
      }
    }
    if (minTokens < LLONG_MAX) part1 += minTokens;

    long long p2x = m.x + 10000000000000;
    long long p2y = m.y + 10000000000000;
    
    long long numeratorX = p2x * m.bdy - p2y * m.bdx;
    long long denomiatorX = m.adx * m.bdy - m.ady * m.bdx;
        
    long long numeratorY = p2x * m.ady - p2y * m.adx;
    long long denomiatorY = m.ady * m.bdx - m.adx * m.bdy;

    if (numeratorX % denomiatorX == 0 && numeratorY % denomiatorY == 0) {
      part2 += 3 * (numeratorX / denomiatorX) + (numeratorY / denomiatorY);
    }
  }

  printf("Part 1: %lld\n", part1);
  printf("Part 2: %lld\n", part2);
}
