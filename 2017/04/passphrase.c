#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compareStrings(const void *a, const void *b) {
    const char *str1 = (const char *)a;
    const char *str2 = (const char *)b;
    return strcmp(str1, str2);
}

int compareLetters(const void* a, const void* b) {
   return (*(char*)a - *(char*)b);
}

int validateList(char words[16][16], int wordCount) {
  qsort(words, wordCount, 16, compareStrings);

  for (int i = 0; i < wordCount-1; i++) {
    if (strcmp(words[i], words[i+1]) == 0) {
      return 0;
    }
  }
  
  return 1;
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

  char line[128];
  while(fgets(line, 128, fp)) {
    char words[16][16];
    int wordCount = 0;

    char *token = 0;
    while ((token = strtok(token ? NULL : line, " \n"))) {
      strcpy(words[wordCount++], token);
    }

    if (validateList(words, wordCount)) part1++;

    for (int i = 0; i < wordCount; i++) {
      qsort(words[i], strlen(words[i]), 1, compareLetters);
    }
    if (validateList(words, wordCount)) part2++;
  }
  fclose(fp);

  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}
