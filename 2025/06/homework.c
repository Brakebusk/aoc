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

  int matrix[4][1000];
  int rc = 0;

  long long part1 = 0;

  char line[4000];
  int mode = 1;
  while(fgets(line, 4000, fp)) {
    char *token = strtok(line, " ");
    int cols = 0;
    do {
      if (token[0] != '\n') {
        if (token[0] == '+' || token[0] == '*') mode = 2;
        if (mode == 1) {
          matrix[rc][cols++] = atoi(token);
        } else {
          long long sum = token[0] == '+' ? 0 : 1;
          for (int r = 0; r < rc; r++) {
            if (token[0] == '+') {
              sum += matrix[r][cols];
            } else {
              sum *= matrix[r][cols];
            }
          }
          cols++;
          part1 += sum;
        }
      }
    } while ((token = strtok(NULL, " \n")));
    rc++;
  }
  fclose(fp);

  printf("Part 1: %lld\n", part1);
}
