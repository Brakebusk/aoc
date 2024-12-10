#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char keypad[3][3] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'}
};

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

  int r = 1, c = 1;
  char part1[8] = {0};

  char line[512];
  while(fgets(line, 512, fp)) {
    for (int i = 0; i < strlen(line); i++) {
      switch (line[i]) {
        case 'U':
          if (r > 0) r--;
          break;
        case 'D':
          if (r < 2) r++;
          break;
        case 'L':
          if (c > 0) c--;
          break;
        case 'R':
          if (c < 2) c++;
          break;
      }
    }
    part1[strlen(part1)] = keypad[r][c];
  }
  fclose(fp);

  printf("Part 1: %s\n", part1);
}
