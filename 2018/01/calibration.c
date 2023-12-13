#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER 500000

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

  int deltas[1024];
  int deltaCount = 0;

  int visited[BUFFER] = {0};
  int visitedCount = 1;

  int part1 = 0;

  char line[32];
  while(fgets(line, 32, fp)) {
    char sign = line[0];
    int magnitude = atoi(&line[1]);

    deltas[deltaCount++] = sign == '+' ? magnitude : -magnitude;
  }
  fclose(fp);

  for (int d = 0; d < deltaCount; d++) {
    part1 += deltas[d];
  } 
  
  int part2 = 0;
  int frequency = 0;
  int dIndex = 0;
  while (!part2) {
    frequency += deltas[dIndex % deltaCount];
    for (int v = 0; v < visitedCount; v++) {
      if (visited[v] == frequency) {
        part2 = frequency;
        break;
      }
    }
    visited[visitedCount++] = frequency;
    if (visitedCount > BUFFER) {
      printf("Ran out of buffer space\n");
      exit(EXIT_FAILURE);
    }
    dIndex++;
  }

  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}
