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

  char slope[512][40];
  int height = 0;
  int width = 0;

  char line[40];
  while(fgets(line, 40, fp)) {
    if (width == 0) width = strlen(line) - 1;
    strncpy(slope[height++], line, width);
  }
  fclose(fp);

  /*
  for (int r = 0; r < height; r++) {
    for (int c = 0; c < width; c++) {
      printf("%c", slope[r][c]);
    }
    printf("\n");
  }
  printf("\n");*/
  
  int part1 = 0;
  int c = 0;
  for (int r = 1; r < height; r++) {
    c += 3;
    if (slope[r][c % width] == '#') part1++;
  }

  printf("Part 1: %d\n", part1);
}
