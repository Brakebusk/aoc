#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isInvalid(long long id) {
  char digits[20] = {0};
  sprintf(digits, "%lld", id);
  size_t length = strlen(digits);

  if (length % 2) return 0;

  for (int c = 0; c < length / 2; c++) {
    if (digits[c] != digits[c + length / 2]) return 0;
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

  char line[600];
  fgets(line, 600, fp);
  fclose(fp);

  long long part1 = 0;

  char *token = strtok(line, ",");
  do {
    long long min, max;
    sscanf(token, "%lld-%lld", &min, &max);

    for (long long id = min; id <= max; id++) {
      if (isInvalid(id)) part1 += id;
    }
  } while ((token = strtok(NULL, ",")));

  printf("Part 1: %lld\n", part1);
}
