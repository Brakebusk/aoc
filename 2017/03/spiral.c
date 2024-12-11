#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int distance(int x, int y) {
  return abs(x) + abs(y);
}

int main(int argc, char *argv[]) {

  int spiral[1000][1000];
  memset(spiral, 0, sizeof(int) * 1000 * 1000);
  int center = 499;

  int r = center, c = center, dir = 2, counter = 1;
  while(1) {
    spiral[r][c] = counter++;
    if (spiral[r][c] == 289326) break;

    switch(dir) {
      case 0:
        if (spiral[r][c-1] == 0) dir = 3;
        break;
      case 1:
        if (spiral[r-1][c] == 0) dir = 0;
        break;
      case 2:
        if (spiral[r][c+1] == 0) dir = 1;
        break;
      case 3:
        if (spiral[r+1][c] == 0) dir = 2;
        break;
    }

    switch(dir) {
      case 0:
        r--;
        break;
      case 1:
        c++;
        break;
      case 2:
        r++;
        break;
      case 3:
        c--;
        break;
    }
  };

  printf("Part 1: %d\n", distance(r-center, c-center));
}
