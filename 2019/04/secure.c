#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int validate(int password, int strict) {
  char digits[7] = {0};
  sprintf(digits, "%d", password);

  int increasing = 1;
  int adjacent = 0;
  for (int d = 0; d < 5; d++) {
    if (digits[d] == digits[d+1]) {
      if (strict) {
        if ((d == 0 || digits[d-1] != digits[d]) && (d == 4 || digits[d+2] != digits[d])) adjacent = 1;
      } else {
        adjacent = 1;
      }
    }
    if (digits[d] > digits[d+1]) {
      increasing = 0;
    }
  }
  if (!adjacent) return 0;
  return increasing;
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

  char line[16];
  fgets(line, 16, fp);
  fclose(fp);

  int min, max;
  sscanf(line, "%d-%d", &min, &max);

  int part1 = 0;
  int part2 = 0;
  for (int try = min; try <= max; try++) {
    part1 += validate(try, 0);
    part2 += validate(try, 1);
  }
  printf("Part1: %d\n", part1);
  printf("Part2: %d\n", part2);
}
