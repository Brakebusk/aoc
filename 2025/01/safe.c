#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int mod(int a, int b) {
  return (a % b + b) % b;
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

  int dial = 50;
  int part1 = 0;
  int part2 = 0;

  char line[8];
  while(fgets(line, 8, fp)) {
    char direction;
    int distance;
    sscanf(line, "%c%d", &direction, &distance);

    while (distance-- > 0) {
      dial += 2 * (direction == 'R') - 1;
      if (mod(dial, 100) == 0) part2++;
    }
    if (mod(dial, 100) == 0) part1++;
  }
  fclose(fp);

  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}
