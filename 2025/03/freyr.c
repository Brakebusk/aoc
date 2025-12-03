#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare(const void* a, const void* b) {
   return (*(int*)a - *(int*)b);
}

int getJoltage(char line[128], int bankSize) {
  int values[100];
  for (int c = 0; c < bankSize; c++) {
    values[c] = line[c] - '0';
  }

  int max = 0;
  for (int a = 0; a < bankSize - 1; a++) {
    for (int b = a + 1; b < bankSize; b++) {
      int v = values[a] * 10 + values[b];
      if (v > max) max = v;
    }
  }

  return max;
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

  char line[128];
  int bankSize = 0;
  while(fgets(line, 128, fp)) {
    if (!bankSize) bankSize = strlen(line) - 1;
    part1 += getJoltage(line, bankSize);
  }
  fclose(fp);

  printf("Part 1: %d\n", part1);
}
