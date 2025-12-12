#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct shape {
  char tiles[3][3];
  int size;
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

  int part1 = 0;

  struct shape shapes[6];
  int sc = 0;

  char line[32];
  int lc = 0;
  int sizesCalculated = 0;
  while(fgets(line, 32, fp)) {
    if (++lc < 31) {
      switch (lc % 5) {
        case 2:
          memcpy(shapes[sc].tiles[0], line, 3);
          break;
        case 3:
          memcpy(shapes[sc].tiles[1], line, 3);
          break;
        case 4:
          memcpy(shapes[sc].tiles[2], line, 3);
          break;
        case 0:
          sc++;
          break;
      }
    } else {
      if (!sizesCalculated) {
        for (int s = 0; s < sc; s++) {
          for (int r = 0; r < 3; r++) {
            for (int c = 0; c < 3; c++) {
              if (shapes[s].tiles[r][c] == '#') {
                shapes[s].size++;
              }
            }
          }
        }
        sizesCalculated = 1;
      }
      int width, height, area;
      int counts[6] = {0};

      char *token = strtok(line, "x");
      width = atoi(token);
      token = strtok(NULL, ":");
      height = atoi(token);
      area = width * height;
      int index = 0;
      while ((token = strtok(NULL, " \n"))) {
        counts[index++] = atoi(token);
      }

      int totalSize = 0;
      for (int c = 0; c < 6; c++) {
        totalSize += counts[c] * shapes[c].size;
      }
      if (totalSize <= area) part1++;
    }
  }
  fclose(fp);

  printf("Part 1: %d\n", part1);
}
