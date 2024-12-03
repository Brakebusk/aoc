#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

int compare(const void* a, const void* b) {
  if (*(double*)a > *(double*)b)
    return 1;
  else if (*(double*)a < *(double*)b)
    return -1;
  else
    return 0;  
}

void printMatrix(char matrix[32][32], int size) {
  printf("Matrix:\n");
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      printf("%c", matrix[i][j]);
    }
    printf("\n");
  }
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

  int size = 0;

  char matrix[32][32];

  char line[32];
  int lineCount = 0;
  while(fgets(line, 32, fp)) {
    if (size == 0) size = strlen(line) - 1;
    memcpy(matrix[lineCount++], line, sizeof(char) * size);
  }
  fclose(fp);

  int part1 = 0;

  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      if (matrix[r][c] == '#') {
        double angles[1024];
        int ac = 0;
        for (int nr = 0; nr < size; nr++) {
          for (int nc = 0; nc < size; nc++) {
            if (nr == r && nc == c || matrix[nr][nc] != '#') continue;

            angles[ac++] = atan2(r - nr, c - nc);
          }
        }
        qsort(angles, ac, sizeof(double), compare);
        
        int unique = 0;
        double prev = DBL_MIN;
        for (int i = 0; i < ac; i++) {
          if (angles[i] != prev) {
            unique++;
            prev = angles[i];
          }
        }
        if (unique > part1) part1 = unique;
      }
    }
  }

  printf("Part 1: %d\n", part1);
}
