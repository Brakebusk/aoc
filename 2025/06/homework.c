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
  char columns[5][4000];
  int rc = 0;

  long long part1 = 0;

  char line[4000];
  int mode = 1;
  while(fgets(line, 4000, fp)) {
    strcpy(columns[rc], line);
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
    if (mode == 1) rc++;
  }
  fclose(fp);

  printf("Part 1: %lld\n", part1);

  long long part2 = 0;

  int components[8] = {0};
  int componentCount = 0;
  for (int c = strlen(columns[0]) - 2; c > -1; c--) {
    int allSpace = 1;
    char digits[8] = {0};
    int dc = 0;
    for (int r = 0; r < rc; r++) {
      if (columns[r][c] != ' ') {
        allSpace = 0;
        digits[dc++] = columns[r][c];
      }
    }
    if (!allSpace) {
      components[componentCount++] = atoi(digits);
      if (columns[rc][c] == '+') {
        for (int comp = 0; comp < componentCount; comp++) {
          part2 += components[comp];
        }
        componentCount = 0;
      } else if (columns[rc][c] == '*') {
        long long product = 1;
        for (int comp = 0; comp < componentCount; comp++) {
          product *= components[comp];
        }
        part2 += product;
        componentCount = 0;
      }
    }
  }

  printf("Part 2: %lld\n", part2);
}
