#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT 289326

int distance(int x, int y) {
  return abs(x) + abs(y);
}

int getAdjacentSum(int spiral[1000][1000], int r, int c) {
  return spiral[r-1][c-1] + spiral[r-1][c] + spiral[r-1][c+1] + 
         spiral[r][c-1] + spiral[r][c] + spiral[r][c+1] + 
         spiral[r+1][c-1] + spiral[r+1][c] + spiral[r+1][c+1];
}

int getSpiral(int part) {
  int spiral[1000][1000];
  memset(spiral, 0, sizeof(int) * 1000 * 1000);
  int center = 499;
  spiral[center][center] = 1;

  int r = center, c = center, dir = 2, counter = 1;
  while(1) {
    spiral[r][c] = part == 1 ? counter++ : getAdjacentSum(spiral, r, c);
    if (part == 1 && spiral[r][c] == INPUT) return distance(r-center, c-center);
    if (part == 2 && spiral[r][c] > INPUT) return spiral[r][c];

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

}

int main(int argc, char *argv[]) {
  printf("Part 1: %d\n", getSpiral(1));
  printf("Part 2: %d\n", getSpiral(2));
}
