#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TEST_MIN 7
#define TEST_MAX 27

struct hail {
  long long px, py, pz;
  long long vx, vy, vz;
};

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    double x;
    double y;
} Velocity;

Point getIntersection(struct hail h1, struct hail h2) {
    Point p1 = {h1.px, h1.py};
    Point p2 = {h2.px, h2.py};
    Velocity v1 = {h1.vx, h1.vy};
    Velocity v2 = {h2.vx, h2.vy};

    int a1 = v1.y;
    int b1 = -v1.x;
    int c1 = a1 * p1.x + b1 * p1.y;

    int a2 = v2.y;
    int b2 = -v2.x;
    int c2 = a2 * p2.x + b2 * p2.y;

    int determinant = a1 * b2 - a2 * b1;

    if (determinant == 0) {
        return (Point){-1, -1};
    } else {
        int x = (b2 * c1 - b1 * c2) / determinant;
        int y = (a1 * c2 - a2 * c1) / determinant;
        if ((x - p1.x) * v1.x < 0 || (y - p1.y) * v1.y < 0 ||
            (x - p2.x) * v2.x < 0 || (y - p2.y) * v2.y < 0) {
          return (Point){-1, -1};
        } else {
          return (Point){x, y};
        }
    }
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

  struct hail hails[300];
  int hailCount = 0;

  char line[80];
  while(fgets(line, 80, fp)) {
    long long values[6];
    sscanf(line, "%lld, %lld, %lld @ %lld, %lld, %lld", &values[0], &values[1], &values[2], &values[3], &values[4], &values[5]);
    hails[hailCount++] = (struct hail) {
      .px = values[0],
      .py = values[1],
      .pz = values[2],
      .vx = values[3],
      .vy = values[4],
      .vz = values[5]
    };
  }
  fclose(fp);

  int part1 = 0;
  for (int i = 0; i < hailCount-1; i++) {
    struct hail h1 = hails[i];

    for (int j = i+1; j < hailCount; j++) {
      struct hail h2 = hails[j];
      Point intersection = getIntersection(h1, h2);

      if (intersection.x > TEST_MIN && 
          intersection.x < TEST_MAX && 
          intersection.y > TEST_MIN && 
          intersection.y < TEST_MAX) part1++;
    }
  }
  printf("Part 1: %d\n", part1);
}
