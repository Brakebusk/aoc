#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 512
#define MID 256

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

  char line[9000];
  fgets(line, 9000, fp);
  fclose(fp);

  int houses[SIZE][SIZE] = {0};

  int x = MID;
  int y = MID;
  houses[MID][MID] = 1;

  for (int c = 0; c < strlen(line); c++) {
    switch (line[c]) {
      case '^':
        y--;
        break;
      case '>':
        x++;
        break;
      case 'v':
        y++;
        break;
      case '<':
        x--;
        break;
      default:
        printf("Unknown character '%c'\n", line[c]);
    }
    houses[y][x]++;
  }

  int part1 = 0;
  for (int r = 0; r < SIZE; r++) {
    for (int c = 0; c < SIZE; c++) {
      if (houses[r][c]) part1++;
    }
  }
  printf("Part 1: %d\n", part1);
}
