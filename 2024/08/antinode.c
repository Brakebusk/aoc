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

  char matrix[64][64];
  char antinodes[64][64];
  char resonantAntinodes[64][64];
  memset(antinodes, 0, sizeof(char) * 64 * 64);
  memset(resonantAntinodes, 0, sizeof(char) * 64 * 64);
  int size = 0;

  char line[64];
  int lc = 0;
  while(fgets(line, 64, fp)) {
    if (size == 0) size = strlen(line) - 1;
    memcpy(matrix[lc++], line, sizeof(char) * size);
  }
  fclose(fp);

  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      for (int r2 = 0; r2 < size; r2++) {
        for (int c2 = 0; c2 < size; c2++) {
          if (r == r2 && c == c2) continue;

          if (matrix[r][c] != '.' && matrix[r][c] == matrix[r2][c2]) {
            int dx = r - r2;
            int dy = c - c2;

            for (int i = -50; i <= 50; i++) {
              int ar = r2 + (dx * i);
              int ac = c2 + (dy * i);

              if (ar >= 0 && ar < size && ac >= 0 && ac < size) {
                resonantAntinodes[ar][ac] = 1;
                if (i == 2) antinodes[ar][ac] = 1;
              }
            }
          }
        }
      }
    }
  }

  int part1 = 0;
  int part2 = 0;
  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      if (antinodes[r][c]) part1++;
      if (resonantAntinodes[r][c]) part2++;
    }
  }
  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}
