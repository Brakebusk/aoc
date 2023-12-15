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

  char line[23000];
  fgets(line, 23000, fp);
  fclose(fp);

  int part1 = 0;

  char *token = 0;
  while ((token = strtok(token ? NULL : line, ","))) {
    int sequenceLength = strlen(token);
    int hash = 0;
    for (int c = 0; c < sequenceLength; c++) {
      hash += token[c];
      hash *= 17;
      hash = hash % 256;
    }
    part1 += hash;
  }
  printf("Part 1: %d\n", part1);
}
