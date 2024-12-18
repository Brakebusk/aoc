#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int min(int a, int b) {
  return a > b ? b : a;
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

  char line[16];
  while(fgets(line, 16, fp)) {
    int l, w, h;
    sscanf(line, "%dx%dx%d", &l, &w, &h);

    int side1 = l * w;
    int side2 = w * h;
    int side3 = h * l;

    int side1Perim = 2 * (l + w);
    int side2Perim = 2 * (w + h);
    int side3Perim = 2 * (h + l);

    part1 += 2 * side1 + 2 * side2 + 2 * side3 + min(min(side1, side2), side3);
    part2 += l * w * h + min(min(side1Perim, side2Perim), side3Perim);
  }
  fclose(fp);

  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}
