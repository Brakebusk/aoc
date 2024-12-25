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

  char locks[400][5];
  int lockCount = 0;
  char keys[400][5];
  int keyCount = 0;

  char image[7][5];
  memset(image, 0, 7 * 5);
  int ir = 0;

  char line[512];
  while(fgets(line, 512, fp)) {
    if (ir == 7) {
      char heights[5];
      memset(heights, -1, 5);
      for (int c = 0; c < 5; c++) {
        for (int r = 0; r < 7; r++) {
          if (image[r][c] == '#') {
            heights[c]++;
          }
        }
      }

      if (image[0][0] == '#') {
        memcpy(locks[lockCount++], heights, 5);
      } else {
        memcpy(keys[keyCount++], heights, 5);
      }

      memset(image, 0, 7 * 5);
      ir = 0;
    } else {
      memcpy(image[ir++], line, 5);
    }
  }
  fclose(fp);

  int part1 = 0;

  for (int i = 0; i < lockCount; i++) {
    char lock[5];
    memcpy(lock, locks[i], 5);

    for (int j = 0; j < keyCount; j++) {
      char key[5];
      memcpy(key, keys[j], 5);

      int overlap = 0;
      for (int c = 0; c < 5; c++) {
        if (lock[c] + key[c] > 5) {
          overlap = 1;
          break;
        }
      }
      if (!overlap) part1++;
    }
  }

  printf("Part 1: %d\n", part1);
}
