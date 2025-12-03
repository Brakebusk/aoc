#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getJoltage(char line[128], int bankSize) {
  int values[100];
  for (int c = 0; c < bankSize; c++) {
    values[c] = line[c] - '0';
  }

  int max = 0;
  for (int a = 0; a < bankSize - 1; a++) {
    for (int b = a + 1; b < bankSize; b++) {
      int v = values[a] * 10 + values[b];
      if (v > max) max = v;
    }
  }

  return max;
}

long long getLargeJoltage(char line[128], int bankSize) {
  char values[128] = {0};
  memcpy(values, line, bankSize);
  long long max = 0;

  char maxDigits[11] = {0};

  for (int a = 0; a < bankSize - 11; a++) {
    char ac = values[a];
    if (ac < maxDigits[0]) continue;
    if (ac > maxDigits[0]) {
      maxDigits[0] = ac;
      memset(&maxDigits[1], 0, 10);
    }
    for (int b = a + 1; b < bankSize - 10; b++) {
      char bc = values[b];
      if (bc < maxDigits[1]) continue;
      if (bc > maxDigits[1]) {
        maxDigits[1] = bc;
        memset(&maxDigits[2], 0, 9);
      }
      for (int c = b + 1; c < bankSize - 9; c++) {
        char cc = values[c];
        if (cc < maxDigits[2]) continue;
        if (cc > maxDigits[2]) {
          maxDigits[2] = cc;
          memset(&maxDigits[3], 0, 8);
        }
        for (int d = c + 1; d < bankSize - 8; d++) {
          char dc = values[d];
          if (dc < maxDigits[3]) continue;
          if (dc > maxDigits[3]) {
            maxDigits[3] = dc;
            memset(&maxDigits[4], 0, 7);
          }
          for (int e = d + 1; e < bankSize - 7; e++) {
            char ec = values[e];
            if (ec < maxDigits[4]) continue;
            if (ec > maxDigits[4]) {
              maxDigits[4] = ec;
              memset(&maxDigits[5], 0, 6);
            }
            for (int f = e + 1; f < bankSize - 6; f++) {
              char fc = values[f];
              if (fc < maxDigits[5]) continue;
              if (fc > maxDigits[5]) {
                maxDigits[5] = fc;
                memset(&maxDigits[6], 0, 5);
              }
              for (int g = f + 1; g < bankSize - 5; g++) {
                char gc = values[g];
                if (gc < maxDigits[6]) continue;
                if (gc > maxDigits[6]) {
                  maxDigits[6] = gc;
                  memset(&maxDigits[7], 0, 4);
                }
                for (int h = g + 1; h < bankSize - 4; h++) {
                  char hc = values[h];
                  if (hc < maxDigits[7]) continue;
                  if (hc > maxDigits[7]) {
                    maxDigits[7] = hc;
                    memset(&maxDigits[8], 0, 3);
                  }
                  for (int i = h + 1; i < bankSize - 3; i++) {
                    char ic = values[i];
                    if (ic < maxDigits[8]) continue;
                    if (ic > maxDigits[8]) {
                      maxDigits[8] = ic;
                      memset(&maxDigits[9], 0, 2);
                    }
                    for (int j = i + 1; j < bankSize - 2; j++) {
                      char jc = values[j];
                      if (jc < maxDigits[9]) continue;
                      if (jc > maxDigits[9]) {
                        maxDigits[9] = jc;
                        memset(&maxDigits[10], 0, 1);
                      }
                      for (int k = j + 1; k < bankSize - 1; k++) {
                        char kc = values[k];
                        if (kc < maxDigits[10]) continue;
                        if (kc > maxDigits[10]) maxDigits[10] = kc;
                        for (int l = k + 1; l < bankSize; l++) {
                          // Surely this is fine
                          char digits[13] = {ac, bc, cc, dc, ec, fc, gc, hc, ic, jc, kc, values[l], 0};
                          long long v = atoll(digits);
                          if (v > max) max = v;
                        } 
                      } 
                    } 
                  } 
                } 
              } 
            } 
          } 
        } 
      }
    }
  }
  return max;
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

  int part1 = 0;
  long long part2 = 0;

  char line[128];
  int bankSize = 0;
  while(fgets(line, 128, fp)) {
    if (!bankSize) bankSize = strlen(line) - 1;
    part1 += getJoltage(line, bankSize);
    part2 += getLargeJoltage(line, bankSize);
  }
  fclose(fp);

  printf("Part 1: %d\n", part1);
  printf("Part 2: %lld\n", part2);
}
