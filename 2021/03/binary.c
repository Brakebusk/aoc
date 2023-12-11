#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int setBit(int num, int index) {
  return num | (1 << index);
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

  char numbers[1024][16];
  int numLength = 0;

  char line[16];
  int numCount = 0;
  while(fgets(line, 16, fp)) {
    if (!numLength) numLength = strlen(line) - 1;

    for (int c = 0; c < strlen(line) - 1; c++) {
      numbers[numCount][c] = line[c] - '0';
    }
    numCount++;
  }
  fclose(fp);

  int gamma = 0;
  int epsilon = 0;

  for (int c = 0; c < numLength; c++) {
    int sum = 0;
    for (int r = 0; r < numCount; r++) {
      sum += numbers[r][c];
    }
    if(sum > numCount / 2) {
      gamma = setBit(gamma, numLength - 1 - c);
    } else {
      epsilon = setBit(epsilon, numLength - 1 - c);
    }
  }

  printf("Part 1: %d\n", gamma * epsilon);
}
