#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char * numbers[] = {
  "one",
  "two",
  "three",
  "four",
  "five",
  "six",
  "seven",
  "eight",
  "nine"
};


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
  int part2 = 0;

  char line[100];
  while(fgets(line, 100, fp)) {
    int stringValueIndexes[100] = {-1};
    for (int n = 0; n < 9; n++) {
      const char *searchBuffer = line;
      while ((searchBuffer = strstr(searchBuffer, numbers[n]))) {
        int pos = (int)(searchBuffer - line);
        if (pos >= 0) {
          stringValueIndexes[pos] = n + 1;
          searchBuffer += strlen(numbers[n]);
        } else {
          break;
        }
      }
    }

    char part1Digits[3] = {'\0', '\0', '\0'};
    char part2Digits[3] = {'\0', '\0', '\0'};

    for (int i = 0; i < strlen(line); i++) {
      int n = stringValueIndexes[i];
      if (n > 0) {
        if (part2Digits[0] == '\0') {
          part2Digits[0] = n + '0';
        }
        part2Digits[1] = n + '0';
      }

      char c = line[i];
      if (c >= '1' && c <= '9') {
        if (part1Digits[0] == '\0') {
          part1Digits[0] = c;
        }
        if (part2Digits[0] == '\0') {
          part2Digits[0] = c;
        }
        part1Digits[1] = c;
        part2Digits[1] = c;
      }
    }
    part1 += atoi(part1Digits);
    part2 += atoi(part2Digits);
  }

  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}

