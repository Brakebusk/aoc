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

  int seats[1024];
  memset(seats, 0, sizeof(int) * 1024);

  char line[16];
  while(fgets(line, 16, fp)) {
    int minRow = 0, maxRow = 127, minCol = 0, maxCol = 7;

    for (int l = 0; l < 7; l++) {
      int diff = 1 + maxRow - minRow;
      switch (line[l]) {
        case 'F':
          maxRow -= diff / 2;
          break;
        case 'B':
          minRow += diff / 2;
          break;
        default:
          printf("Unknown value '%c'\n", line[l]);
          exit(EXIT_FAILURE);
      }
    }

    for (int l = 7; l < 10; l++) {
      int diff = 1 + maxCol - minCol;
      switch (line[l]) {
        case 'R':
          minCol += diff / 2;
          break;
        case 'L':
          maxCol -= diff / 2;
          break;
        default:
          printf("Unknown value '%c'\n", line[l]);
          exit(EXIT_FAILURE);
      }
    }
    
    int id = minRow * 8 + minCol;
    seats[id] = 1;
    if (id > part1) part1 = id;
  }
  fclose(fp);

  printf("Part 1: %d\n", part1);

  for (int i = 1; i < 1024-1; i++) {
    if (seats[i-1] && !seats[i] && seats[i+1]) {
      printf("Part 2: %d\n", i);
      break;
    }
  }
}
