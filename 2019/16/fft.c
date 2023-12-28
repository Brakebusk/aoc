#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int basePattern[4] = {0, 1, 0, -1};
int getPatternValue(int digit, int index) {
  int repeats = digit + 1;

  return basePattern[((index + 1) / repeats) % 4];
}

void printDigits(int *digits, int digitCount, int offset) {
  for (int i = offset; i < digitCount+offset; i++) printf("%d", digits[i]);
  printf("\n");
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

  char line[800];
  fgets(line, 800, fp);
  fclose(fp);

  int digitCount = strlen(line);
  int *digits = malloc(digitCount * sizeof(int) * 10001);
  for (int d = 0; d < digitCount; d++) digits[d] = line[d] - '0';
  for (int r = 0; r < 10000; r++) {
    memcpy(&digits[(r+1) * digitCount], digits, sizeof(int) * digitCount);
  }

  int *output = malloc(digitCount * sizeof(int) * 10001);

  char offsetDigits[8];
  strncpy(offsetDigits, line, 7);
  int offset = atoi(offsetDigits);

  int trueDigitCount = digitCount*10000;

  for (int i = 0; i < 100; i++) {
    printf("Round #%d\n", i+1);
    for (int d = 0; d < trueDigitCount; d++) {
      if (d % 10000 == 0) printf("  -> digit %d\n", d);
      int v = 0;
      for (int j = 0; j < trueDigitCount; j++) {
        v += getPatternValue(d, j) * digits[j];
      }
      output[d] = abs(v) % 10;
    }

    memcpy(digits, output, trueDigitCount * sizeof(int));
  }
  printf("Part 1: ");
  printDigits(digits, 8, 0);
  printf("Part 2 (offset %d): ", offset);
  printDigits(digits, 8, offset);
  free(digits);
}
