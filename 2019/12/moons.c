#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long gcd(long long a, long long b) {
    if (a == 0) {
        return b;
    }
    return gcd(b % a, a);
}

long long lcm(int arr[], int n) {
    long long ans = arr[0];
    for (int i = 1; i < n; i++) {
        ans = (((arr[i] * ans)) / (gcd(arr[i], ans)));
    }
    return ans;
}

struct moon {
  int x, y, z;
  int vx, vy, vz;
};

struct dimension {
  int moon[4];
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

  struct moon moons[4];

  struct dimension histories[3][10000];
  memset(histories, 0, sizeof(struct dimension) * 3 * 10000);
  int hIndex = 0;

  int repeatingIndex[3] = {0};
  int allRepeating = 0;

  char line[32];
  int lc = 0;
  while(fgets(line, 32, fp)) {
    int x, y, z;
    sscanf(line, "<x=%d, y=%d, z=%d>", &x, &y, &z);
    moons[lc] = (struct moon) {
      .x = x,
      .y = y,
      .z = z,
      .vx = 0,
      .vy = 0,
      .vz = 0
    };
    histories[0][0].moon[lc] = x;
    histories[1][0].moon[lc] = y;
    histories[2][0].moon[lc] = z;
    lc++;
  }
  fclose(fp);
  hIndex++;

  for (int step = 0; step < 10000 || allRepeating < 3; step++) {
    for (int m1 = 0; m1 < 3; m1++) {
      for (int m2 = m1 + 1; m2 < 4; m2++) {
        if (moons[m1].x > moons[m2].x) {
          moons[m1].vx--;
          moons[m2].vx++;
        } else if (moons[m1].x < moons[m2].x) {
          moons[m1].vx++;
          moons[m2].vx--;
        }
        if (moons[m1].y > moons[m2].y) {
          moons[m1].vy--;
          moons[m2].vy++;
        } else if (moons[m1].y < moons[m2].y) {
          moons[m1].vy++;
          moons[m2].vy--;
        }
        if (moons[m1].z > moons[m2].z) {
          moons[m1].vz--;
          moons[m2].vz++;
        } else if (moons[m1].z < moons[m2].z) {
          moons[m1].vz++;
          moons[m2].vz--;
        }
      }
    }
    for (int m = 0; m < 4; m++) {
      moons[m].x += moons[m].vx;
      moons[m].y += moons[m].vy;
      moons[m].z += moons[m].vz;

      histories[0][hIndex].moon[m] = moons[m].x;
      histories[1][hIndex].moon[m] = moons[m].y;
      histories[2][hIndex].moon[m] = moons[m].z;
    }
    
    for (int d = 0; d < 3; d++) {
      if (!repeatingIndex[d]) {
        for (int h = 0; h < hIndex-1; h++) {
          if (histories[d][h].moon[0] == histories[d][hIndex].moon[0] &&
              histories[d][h].moon[1] == histories[d][hIndex].moon[1] &&
              histories[d][h].moon[2] == histories[d][hIndex].moon[2] &&
              histories[d][h].moon[3] == histories[d][hIndex].moon[3]) {
            repeatingIndex[d] = h;
            allRepeating++;
            break;
          }
        }
      }
    }

    hIndex++;

    if (step == 999) {
      int part1 = 0;
      for (int m = 0; m < 4; m++) {
        int pot = abs(moons[m].x) + abs(moons[m].y) + abs(moons[m].z);
        int kin = abs(moons[m].vx) + abs(moons[m].vy) + abs(moons[m].vz);
        part1 += pot * kin;
      }
      printf("Part 1: %d\n", part1);
    }
  }

  printf("Got %d %d %d\n", repeatingIndex[0], repeatingIndex[1], repeatingIndex[2]);

  printf("Part 2: %lld\n", lcm(repeatingIndex, 3));
}
