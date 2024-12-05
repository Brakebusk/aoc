#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int basePattern[4] = {0, 1, 0, -1};
int getPatternValue(int digit, int index) {
  int repeats = digit + 1;

  return basePattern[((index + 1) / repeats) % 4];
}

void printDigits(int digits[800], int digitCount) {
  for (int i = 0; i < digitCount; i++) printf("%d", digits[i]);
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

  int digits[800];
  int digitCount = strlen(line);
  for (int d = 0; d < digitCount; d++) digits[d] = line[d] - '0';

  int output[800];

  for (int i = 0; i < 100; i++) {
    for (int d = 0; d < digitCount; d++) {
      int v = 0;
      for (int j = d; j < digitCount; j++) {
        v += getPatternValue(d, j) * digits[j];
      }
      output[d] = abs(v) % 10;
    }

    for (int d = 0; d < digitCount; d++) {
      digits[d] = output[d];
    }
  }
  printf("Part 1: ");
  printDigits(digits, 8);

  char offsetBuffer[8] = {0};
  strncpy(offsetBuffer, line, 7);
  int offset = atoi(offsetBuffer);

  int endOffset = (10000 * digitCount) - offset;

  char reverse[800];
  for (int i = 0; i < digitCount; i++) reverse[i] = line[digitCount-1-i];

  int *lastDigits = malloc(endOffset * sizeof(int));
  for (int i = endOffset - 1; i >= 0; i--) {
    lastDigits[i] = reverse[(endOffset - 1 - i) % digitCount]-'0';
  }
  int *lastOutput = malloc(endOffset * sizeof(int));

  for (int i = 0; i < 100; i++) {
    for (int d = endOffset - 1; d >= 0; d--) {
      if (d == endOffset - 1) {
        lastOutput[endOffset-1] = lastDigits[endOffset-1];
      } else {
        lastOutput[d] = (lastDigits[d] + lastOutput[d+1]) % 10;
      }
    }

    for (int d = 0; d < endOffset; d++) {
      lastDigits[d] = lastOutput[d];
    }

  }

  printf("Part 2: ");
  printDigits(lastDigits, 8);
}