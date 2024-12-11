#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

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
  while(fgets(line, 128, fp)) {
    int values[32];
    int count = 0;

    char *token;
    while ((token = strtok(token ? NULL : line, " "))) {
      values[count++] = atoi(token);
    }
    
    int min = INT_MAX;
    int max = INT_MIN;
    for (int i = 0; i < count; i++) {
      if (values[i] < min) min = values[i];
      if (values[i] > max) max = values[i];
    }
    part1 += max - min;
  }
  fclose(fp);

  printf("Part 1: %d\n", part1);
}
