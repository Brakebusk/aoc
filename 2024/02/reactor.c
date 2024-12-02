#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check(int values[], int length) {
  // Negative if increasing
  int direction = 0;
  for (int i = 0; i < length - 1; i++) {
    if (direction == 0) {
      direction = values[i] - values[i+1];
    }

    int diff = abs(values[i] - values[i+1]);
    
    if (diff > 3 || diff < 1) return 0;
    if (direction < 0 && values[i] > values[i+1]) return 0;
    if (direction > 0 && values[i] < values[i+1]) return 0;
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
  int part2 = 0;

  char line[32];
  while(fgets(line, 32, fp)) {
    int values[16];
    int length = 0;

    char *token = strtok(line, " ");
    while( token != NULL ) {
      values[length++] = atoi(token);
      token = strtok(NULL, " ");
    }

    int valid = check(values, length);
    if (valid) {
      part1++;
      part2++;
    } else {
      for (int remove = 0; remove < length; remove++) {
        int copyLength = 0;
        int copy[16];
        for (int i = 0; i < length; i++) {
          if (i != remove) copy[copyLength++] = values[i];
        }
        if (check(copy, copyLength)) {
          part2++;
          break;
        }
      }
    }
  }
  fclose(fp);

  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}
