#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <limits.h>

int compare(const void* a, const void* b) {
  if (*(double*)a > *(double*)b)
    return 1;
  else if (*(double*)a < *(double*)b)
    return -1;
  else
    return 0;  
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

  int height = 0, width = 0;

  char matrix[32][32];

  char line[32];
  while(fgets(line, 32, fp)) {
    if (width == 0) width = strlen(line) - 1;
    memcpy(matrix[height++], line, sizeof(char) * width);
  }
  fclose(fp);

  int part1 = 0;

  int mr, mc, sac;
  double sortedAngles[1024];

  for (int r = 0; r < height; r++) {
    for (int c = 0; c < width; c++) {
      if (matrix[r][c] == '#') {
        double angles[1024];
        int ac = 0;
        for (int nr = 0; nr < height; nr++) {
          for (int nc = 0; nc < width; nc++) {
            if ((nr == r && nc == c) || matrix[nr][nc] != '#') continue;

            angles[ac++] = atan2(nr - r, nc - c);
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
        if (unique > part1) {
          part1 = unique;
          mr = r;
          mc = c;
          sac = ac;
          memcpy(sortedAngles, angles, sizeof(double) * ac);
        }
      }
    }
  }

  matrix[mr][mc] = 'X';

  int anglesIndex;
  for (int i = 0; i < sac; i++) {
    if (sortedAngles[i] >= -M_PI/2) {
      anglesIndex = i;
      break;
    }
  }
  
  int singed = 0;
  int lr, lc;
  while (singed < 200) {
    int equal = 0;
    for (int i = anglesIndex; i < sac; i++) {
      if (sortedAngles[i] == sortedAngles[anglesIndex]) equal++;
    }
    int rc = -1, cc = -1, distance;
    for (int r = 0; r < height; r++) {
      for (int c = 0; c < width; c++) {
        if (matrix[r][c] == '#' && sortedAngles[anglesIndex] == atan2(r - mr, c - mc)) {
          int dist = abs(r - mr) + abs(c - mc);
          if (rc == -1 || dist < distance) {
            rc = r;
            cc = c;
            distance = dist;
          }
        }
      }
    }

    matrix[rc][cc] = '.';
    lr = rc;
    lc = cc;
    for (int i = anglesIndex; i < sac - 1; i++) {
      sortedAngles[i] = sortedAngles[i+1];
    }
    sac--;

    anglesIndex = (anglesIndex + equal - 1) % sac;
    singed++;
  }

  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", lc * 100 + lr);
}
