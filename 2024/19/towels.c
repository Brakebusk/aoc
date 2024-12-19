#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>

char patterns[512][16];
int patternCount = 0; 

long long validate(char design[64]) {
  if (strlen(design) == 0) return 1;

  ENTRY entry;
  entry.key = malloc(64);
  sprintf(entry.key, "%s", design);
  
  ENTRY *found = hsearch(entry, FIND);
  if (found != NULL) {
    free(entry.key);
    return *(long long *)found->data;
  }

  long long possibleArrangements = 0;
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

    if (matches) possibleArrangements += validate(&design[pLength]);
  }

  entry.data = malloc(sizeof(long long));
  *(long long *)entry.data = possibleArrangements;
  hsearch(entry, ENTER);
  return possibleArrangements;
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

  long long part1 = 0;
  long long part2 = 0;

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
      long long arrangements = validate(design);
      if (arrangements) part1++;
      part2 += arrangements;
    }
    mode++;
  }
  fclose(fp);

  printf("Part 1: %lld\n", part1);
  printf("Part 2: %lld\n", part2);

  hdestroy();
}
