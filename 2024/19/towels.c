#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>

char patterns[512][16];
int patternCount = 0; 

int validate(char design[64]) {
  if (strlen(design) == 0) return 1;

  ENTRY entry;
  entry.key = malloc(64);
  sprintf(entry.key, "%s", design);
  
  ENTRY *found = hsearch(entry, FIND);
  if (found != NULL) {
    free(entry.key);
    return *(int *)found->data;
  }

  for (int i = 0; i < patternCount; i++) {
    char *pattern = patterns[i];
    int pLength = strlen(pattern);
    int matches = 1;
    for (int c = 0; c < pLength; c++) {
      if (design[c] != pattern[c]) {
        matches = 0;
        break;
      }
    }

    if (matches && validate(&design[pLength])) {
      entry.data = malloc(sizeof(int));
      *(int *)entry.data = 1;
      hsearch(entry, ENTER);
      return 1;
    }
  }

  entry.data = malloc(sizeof(int));
  *(int *)entry.data = 0;
  hsearch(entry, ENTER);
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

  hcreate(20000);

  int part1 = 0;

  char line[3000];
  int mode = 0;
  while(fgets(line, 3000, fp)) {
    if (mode == 0) {
      char *token = 0;
      while ((token = strtok(token ? NULL : line, ", \n"))) {
        strcpy(patterns[patternCount++], token);
      }
    } else if (mode > 1) {
      int length = strlen(line);
      if (line[length-1] == '\n') length--;

      char design[64];
      memset(design, 0, 64);
      strncpy(design, line, length);
      part1 += validate(design);
    }
    mode++;
  }
  fclose(fp);

  printf("Part 1: %d\n", part1);

  hdestroy();
}
