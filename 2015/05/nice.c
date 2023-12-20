#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isVowel(char c) {
  if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') return 1;
  return 0;
}

int isNice(char *string) {
  int length = strlen(string);
  
  int vowelCount = 0;
  for (int c = 0; c < length && vowelCount < 3; c++) {
    vowelCount += isVowel(string[c]);
  }
  if (vowelCount < 3) return 0;

  int repeating = 0;
  for (int c = 0; c < length - 1; c++) {
    if (string[c] == string[c+1]) {
      repeating = 1;
    }
    if ((string[c] == 'a' && string[c+1] == 'b') || 
        (string[c] == 'c' && string[c+1] == 'd') || 
        (string[c] == 'p' && string[c+1] == 'q') || 
        (string[c] == 'x' && string[c+1] == 'y')) return 0;
  }
  return repeating;
}

int isNiceV2(char *string) {
  int length = strlen(string);

  int hasPair = 0;
  for (int c = 0; c < length - 3; c++) {
    char p11 = string[c];
    char p12 = string[c+1];
    for (int c2 = c + 2; c2 < length - 1; c2++) {
      if (p11 == string[c2] && p12 == string[c2+1]) {
        hasPair = 1;
        break;
      }
    }
  }
  if (!hasPair) return 0;

  for (int c = 0; c < length - 2; c++) {
    if (string[c] == string[c+2]) {
      return 1;
    }
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

  int part1 = 0;
  int part2 = 0;

  char line[20];
  while(fgets(line, 20, fp)) {
    char *token = strtok(line, "\n");
    part1 += isNice(token);
    part2 += isNiceV2(token);
  }
  fclose(fp);

  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}
