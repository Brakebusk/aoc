#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct junction {
  int x, y, z, circuit;
};

long long square(long long n) {
  return n * n;
}

long long getDistance(struct junction a, struct junction b) {
  return square(a.x - b.x) + square(a.y - b.y) + square(a.z - b.z);
}

int compare(const void* a, const void* b) {
   return (*(int*)b - *(int*)a);
}

int makeNextConnection(struct junction junctions[1024], int jc, long long *distances[1024]) {
  long long minDistance = LLONG_MAX;
  int selectedA, selectedB;
  for (int a = 0; a < jc; a++) {
    for (int b = a + 1; b < jc; b++) {
      if (distances[a][b] != 0 && minDistance > distances[a][b]) {
        minDistance = distances[a][b];
        selectedA = a;
        selectedB = b;
      }
    }
  }

  distances[selectedA][selectedB] = 0;

  if (junctions[selectedA].circuit != junctions[selectedB].circuit) {
    int migrateFrom = junctions[selectedB].circuit;
    int migrateTo = junctions[selectedA].circuit;
    int equalCount = 0;
    
    for (int j = 0; j < jc; j++) {
      if (junctions[j].circuit == migrateFrom) {
        junctions[j].circuit = migrateTo;
        equalCount++;
      } else if (junctions[j].circuit == migrateTo) equalCount++;
    }
    
    if (equalCount == jc) return junctions[selectedA].x * junctions[selectedB].x;
  }
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("[ERROR] Missing parameter <filename>\n");
    exit(EXIT_FAILURE);
  }

  char *filename = argv[1];
  int rounds = argc == 3 ? atoi(argv[2]) : 1000;

  FILE *fp = NULL;
  if ((fp = fopen(filename, "r")) == NULL) {
      printf("[ERROR] Failed to open file %s\n", filename);
      exit(EXIT_FAILURE);
  }

  struct junction junctions[1024];
  int jc = 0;

  char line[24];
  while(fgets(line, 24, fp)) {
    sscanf(line, "%d,%d,%d", &junctions[jc].x, &junctions[jc].y, &junctions[jc].z);
    junctions[jc].circuit = jc;
    jc++;
  }
  fclose(fp);

  long long *distances[1024];
  for (int d = 0; d < 1024; d++) distances[d] = malloc(sizeof(long long) * 1024);

  for (int a = 0; a < jc; a++) {
    for (int b = a + 1; b < jc; b++) {
      distances[a][b] = getDistance(junctions[a], junctions[b]);
    }
  }

  for (int r = 0; r < rounds; r++) makeNextConnection(junctions, jc, distances);

  int lengths[1024] = {0};
  for (int j = 0; j < jc; j++) lengths[junctions[j].circuit]++;
  qsort(lengths, 1024, sizeof(int), compare);

  int part1 = lengths[0] * lengths[1] * lengths[2];
  printf("Part 1: %d\n", part1);

  int part2;
  while (!(part2 = makeNextConnection(junctions, jc, distances))) {
    continue;
  }
  printf("Part 2: %d\n", part2);

  for (int d = 0; d < 1024; d++) free(distances[d]);
}
