#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void drawCRT(int cycle, int x) {
  int currentPosition = (cycle - 1) % 40;
  if (currentPosition == 0) printf("\n");
  printf("%c", abs(currentPosition - x) <= 1 ? '#' : '.');
}

int checkInteresting(int cycle, int x) {
  drawCRT(cycle, x);
  if ((cycle - 20) % 40 == 0) {
    return cycle * x;
  }
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

  int x = 1;
  int cycle = 1;
  int interestingSum = 0;

  char line[20];
  while(fgets(line, 20, fp)) {
    char instruction[5] = {0}, value[5] = {0};
    sscanf(line, "%s %[^\n]", instruction, value);

    if (strcmp(instruction, "addx") == 0) {
      interestingSum += checkInteresting(cycle++, x);
      interestingSum += checkInteresting(cycle++, x);
      x += atoi(value);
    } else if (strcmp(instruction, "noop") == 0) {
      interestingSum += checkInteresting(cycle++, x);
    } else {
      printf("[ERROR] Invalid instruction '%s'\n", instruction);
      exit(EXIT_FAILURE);
    }
  }

  fclose(fp);

  printf("\n\nPart 1: %d\n", interestingSum);
}