#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct junction {
  int x, y, z;
};

struct circuit {
  int length;
  int junctionIndexes[1024];
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
  struct circuit* circuits = malloc(sizeof(struct circuit) * 1024);
  int cc = 0;

  char line[24];
  while(fgets(line, 24, fp)) {
    sscanf(line, "%d,%d,%d", &junctions[cc].x, &junctions[cc].y, &junctions[cc].z);
    circuits[cc].junctionIndexes[0] = cc;
    circuits[cc++].length = 1;
  }
  fclose(fp);
  int jc = cc;

  long long *distances[1024];
  for (int d = 0; d < 1024; d++) distances[d] = malloc(sizeof(long long) * 1024);

  for (int a = 0; a < jc; a++) {
    for (int b = 0; b < jc; b++) {
      if (a != b) {
        distances[a][b] = getDistance(junctions[a], junctions[b]);
      } else distances[a][b] = 0;
    }
  }

  for (int r = 0; r < rounds; r++) {
    long long minDistance = LLONG_MAX;
    int selectedA, selectedB;
    for (int a = 0; a < jc; a++) {
      for (int b = 0; b < jc; b++) {
        if (distances[a][b] != 0 && minDistance > distances[a][b]) {
          minDistance = distances[a][b];
          selectedA = a;
          selectedB = b;
        }
      }
    }

    distances[selectedA][selectedB] = 0;
    distances[selectedB][selectedA] = 0;
    int aCircuit, bCircuit;
    for (int c = 0; c < cc; c++) {
      for (int j = 0; j < circuits[c].length; j++) {
        if (circuits[c].junctionIndexes[j] == selectedA) aCircuit = c;
        if (circuits[c].junctionIndexes[j] == selectedB) bCircuit = c;
      }
    }
    if (aCircuit != bCircuit) {
      memcpy(&circuits[aCircuit].junctionIndexes[circuits[aCircuit].length], circuits[bCircuit].junctionIndexes, sizeof(int) * circuits[bCircuit].length);
      circuits[aCircuit].length += circuits[bCircuit].length;
      circuits[bCircuit].length = 0;
    }
  }

  int lengths[1024] = {0};
  for (int c = 0; c < cc; c++) {
    lengths[c] = circuits[c].length;
  }
  qsort(lengths, cc, sizeof(int), compare);

  int part1 = lengths[0] * lengths[1] * lengths[2];
  printf("Part 1: %d\n", part1);
}
