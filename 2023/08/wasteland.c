#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_char_as_binary(char ch) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (ch >> i) & 1);
    }
    printf("\n");
}

void print_unsigned_short_as_binary(unsigned short us) {
    for (int i = 15; i >= 0; i--) {
        printf("%d", (us >> i) & 1);
    }
    printf("\n");
}

struct node {
  unsigned short next[2];
};

char letterToNumber(char letter) {
  return letter - 'A';
}

unsigned short nameToCoordinate(char name[]) {
  char n1 = letterToNumber(name[0]);
  char n2 = letterToNumber(name[1]);
  char n3 = letterToNumber(name[2]);

  unsigned short result = ((n1 & 0b00011111) << 10) | ((n2 & 0b00011111) << 5) | (n3 & 0b00011111);
  return result;
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

  int steps[512] = {0};
  int stepCount = 0;

  struct node grid[27000];

  char line[512];
  int lineCount = 0;
  while(fgets(line, 512, fp)) {
    if (lineCount == 0) {
      for (int i = 0; i < strlen(line) - 1; i++) steps[stepCount++] = line[i] == 'R';
    } else if (lineCount > 1) {
      char from[4] = {0}, left[4] = {0}, right[4] = {0};
      sscanf(line, "%3s = (%3s, %3s)", from, left, right);
      grid[nameToCoordinate(from)] = (struct node) {
        .next = {nameToCoordinate(left), nameToCoordinate(right)}
      };
    }
    lineCount++;
  }
  fclose(fp);

  int step = 0;
  unsigned short location = 0;
  unsigned short goal = nameToCoordinate("ZZZ");
  while (location != goal) {
    int nextStep = steps[step++ % stepCount];
    location = grid[location].next[nextStep];
  }
  printf("Part 1: %d\n", step);
}
