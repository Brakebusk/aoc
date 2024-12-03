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

const int HISTORY_LENGTH = 1000000;

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

  struct moon* history1 = malloc(sizeof(struct moon) * HISTORY_LENGTH);
  memset(history1, 0, sizeof(struct moon) * HISTORY_LENGTH);
  struct moon* history2 = malloc(sizeof(struct moon) * HISTORY_LENGTH);
  memset(history2, 0, sizeof(struct moon) * HISTORY_LENGTH);
  struct moon* history3 = malloc(sizeof(struct moon) * HISTORY_LENGTH);
  memset(history3, 0, sizeof(struct moon) * HISTORY_LENGTH);
  struct moon* history4 = malloc(sizeof(struct moon) * HISTORY_LENGTH);
  memset(history4, 0, sizeof(struct moon) * HISTORY_LENGTH);

  struct moon* histories[4] = {history1, history2, history3, history4};
  int hIndex = 0;

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
    memcpy(histories[lc], &moons[lc], sizeof(struct moon));
    lc++;
  }
  fclose(fp);
  hIndex++;

  for (int step = 0; step < HISTORY_LENGTH-1; step++) {
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

      memcpy(&histories[m][hIndex], &moons[m], sizeof(struct moon));
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
  
  int xRep, yRep, zRep;
  for (int h = 1; h < HISTORY_LENGTH; h++) {
    if (histories[0][h].x == histories[0][0].x && histories[0][h].vx == 0 &&
        histories[1][h].x == histories[1][0].x && histories[1][h].vx == 0 &&
        histories[2][h].x == histories[2][0].x && histories[2][h].vx == 0 &&
        histories[3][h].x == histories[3][0].x && histories[3][h].vx == 0) {
      xRep = h;
      break;
    }
  }
  for (int h = 1; h < HISTORY_LENGTH; h++) {
    if (histories[0][h].y == histories[0][0].y && histories[0][h].vy == 0 &&
        histories[1][h].y == histories[1][0].y && histories[1][h].vy == 0 &&
        histories[2][h].y == histories[2][0].y && histories[2][h].vy == 0 &&
        histories[3][h].y == histories[3][0].y && histories[3][h].vy == 0) {
      yRep = h;
      break;
    }
  }
  for (int h = 1; h < HISTORY_LENGTH; h++) {
    if (histories[0][h].z == histories[0][0].z && histories[0][h].vz == 0 &&
        histories[1][h].z == histories[1][0].z && histories[1][h].vz == 0 &&
        histories[2][h].z == histories[2][0].z && histories[2][h].vz == 0 &&
        histories[3][h].z == histories[3][0].z && histories[3][h].vz == 0) {
      zRep = h;
      break;
    }
  }

  printf("Part 2: %lld\n", lcm((int[]){xRep, yRep, zRep}, 3));

  free(history1);
  free(history2);
  free(history3);
  free(history4);
}
