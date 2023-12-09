#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int areZero(int sequence[], int sequenceLength) {
  for (int i = 0; i < sequenceLength; i++) {
    if (sequence[i] != 0) return 0;
  }
  return 1;
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

  char line[256];
  while(fgets(line, 256, fp)) {
    int grid[128][128] = {0};
    char *token = strtok(line, " ");
    int sequenceLength[128] = {0};
    grid[0][sequenceLength[0]++] = atoi(token);
    while ((token = strtok(NULL, " "))) {
      grid[0][sequenceLength[0]++] = atoi(token);
    }

    int height = 0;
    while (!areZero(grid[height], sequenceLength[height])) {
      height++;
      sequenceLength[height] = sequenceLength[height-1]-1;
      for (int i = 0; i < sequenceLength[height]; i++) {
        grid[height][i] = grid[height-1][i+1] - grid[height-1][i];
      }
    }
    grid[height][sequenceLength[height]++] = 0;

    for (int i = height-1; i >= 0; i--) {
      grid[i][sequenceLength[i]] = grid[i][sequenceLength[i]-1] + grid[i+1][sequenceLength[i+1]-1];
      sequenceLength[i]++;
    }

    part1 += grid[0][sequenceLength[0]-1];
  }
  fclose(fp);

  printf("Part 1: %d\n", part1);
}
