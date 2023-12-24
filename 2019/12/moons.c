#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct moon {
  int x, y, z;
  int vx, vy, vz;
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

  char line[32];
  int lc = 0;
  while(fgets(line, 32, fp)) {
    int x, y, z;
    sscanf(line, "<x=%d, y=%d, z=%d>", &x, &y, &z);
    moons[lc++] = (struct moon) {
      .x = x,
      .y = y,
      .z = z,
      .vx = 0,
      .vy = 0,
      .vz = 0
    };
  }
  fclose(fp);

  for (int step = 0; step < 1000; step++) {
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
    }
  }

  int part1 = 0;
  for (int m = 0; m < 4; m++) {
    int pot = abs(moons[m].x) + abs(moons[m].y) + abs(moons[m].z);
    int kin = abs(moons[m].vx) + abs(moons[m].vy) + abs(moons[m].vz);
    part1 += pot * kin;
  }
  printf("Part 1: %d\n", part1);
}
