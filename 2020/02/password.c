#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

  char line[48];
  while(fgets(line, 48, fp)) {
    int min, max;
    char letter;
    char password[32] = {0};
    sscanf(line, "%d-%d %c: %s", &min, &max, &letter, password);

    int occurrences = 0;
    for (int i = 0; i < strlen(password); i++) {
      if (password[i] == letter) occurrences++;
    }
    if (occurrences >= min && occurrences <= max) part1++;
  }
  fclose(fp);

  printf("Part 1: %d\n", part1);
}
