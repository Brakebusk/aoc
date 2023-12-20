#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int fuelWithMass(int v) {
  int t = 0;

  int f = v;
  while ((f = f / 3 - 2) > 0) t += f;

  return t;
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

  int part1 = 0;
  int part2 = 0;

  char line[10];
  while(fgets(line, 10, fp)) {
    int value = atoi(line);
    part1 += value / 3 - 2;
    part2 += fuelWithMass(value);
  }
  fclose(fp);

  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}
