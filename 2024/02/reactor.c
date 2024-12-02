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

  int part1 = 0;

  char line[32];
  while(fgets(line, 32, fp)) {
    int values[16];
    int length = 0;

    char *token = strtok(line, " ");
    while( token != NULL ) {
      values[length++] = atoi(token);
      token = strtok(NULL, " ");
    }

    // Negative if increasing
    int direction = 0;
    int valid = 1;
    for (int i = 0; i < length - 1; i++) {
      if (direction == 0) {
        direction = values[i] - values[i+1];
      }

      int diff = abs(values[i] - values[i+1]);
      if (diff > 3 || diff < 1) {
        valid = 0;
        break;
      }

      if (direction < 0 && values[i] > values[i+1]) {
        valid = 0;
        break;
      }
      if (direction > 0 && values[i] < values[i+1]) {
        valid = 0;
        break;
      }

    }
    part1 += valid;
  }
  fclose(fp);

  printf("Part 1: %d\n", part1);
}
