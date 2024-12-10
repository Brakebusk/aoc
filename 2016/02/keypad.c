#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char keypad[3][3] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'}
};

const char weirdKeypad[5][5] = {
  {0, 0, '1', 0, 0},
  {0, '2', '3', '4', 0},
  {'5', '6', '7', '8', '9'},
  {0, 'A', 'B', 'C', 0},
  {0, 0, 'D', 0, 0}
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

  int r1 = 1, c1 = 1, r2 = 2, c2 = 0;
  char part1[8] = {0};
  char part2[8] = {0};

  char line[512];
  while(fgets(line, 512, fp)) {
    for (int i = 0; i < strlen(line); i++) {
      switch (line[i]) {
        case 'U':
          if (r1 > 0) r1--;
          if (r2 > 0 && weirdKeypad[r2-1][c2] != 0) r2--;
          break;
        case 'D':
          if (r1 < 2) r1++;
          if (r2 < 4 && weirdKeypad[r2+1][c2] != 0) r2++;
          break;
        case 'L':
          if (c1 > 0) c1--;
          if (c2 > 0 && weirdKeypad[r2][c2-1] != 0) c2--;
          break;
        case 'R':
          if (c1 < 2) c1++;
          if (c2 < 4 && weirdKeypad[r2][c2+1] != 0) c2++;
          break;
      }
    }
    part1[strlen(part1)] = keypad[r1][c1];
    part2[strlen(part2)] = weirdKeypad[r2][c2];
  }
  fclose(fp);

  printf("Part 1: %s\n", part1);
  printf("Part 2: %s\n", part2);
}
