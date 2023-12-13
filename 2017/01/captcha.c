#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

  char line[2049];
  fgets(line, 2049, fp);
  fclose(fp);
  int digitCount = strlen(line);

  int part1 = 0;

  for (int d = 0; d < digitCount; d++) {
    char digit = line[d];
    char nextDigit = line[(d+1) % digitCount];
    if (digit == nextDigit) part1 += digit - '0';
  }

  printf("Part 1: %d\n", part1);
}
