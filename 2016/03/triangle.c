#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare(const void* a, const void* b) {
   return (*(int*)a - *(int*)b);
}

int check(int a, int b, int c) {
  int values[3] = {a, b, c};
  qsort(values, 3, sizeof(int), compare);
  return values[0] + values[1] > values[2];
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

  int grid[2000][3];
  int length = 0;

  int part1 = 0;
  int part2 = 0;

  char line[20];
  while(fgets(line, 20, fp)) {
    int a, b, c;
    sscanf(line, "%d %d %d", &a, &b, &c);
    grid[length][0] = a;
    grid[length][1] = b;
    grid[length++][2] = c;
    part1 += check(a, b, c);
  }
  fclose(fp);

  for (int r = 0; r < length; r += 3) {
    part2 += check(grid[r][0], grid[r+1][0], grid[r+2][0]) +
             check(grid[r][1], grid[r+1][1], grid[r+2][1]) + 
             check(grid[r][2], grid[r+1][2], grid[r+2][2]);
  }

  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}
