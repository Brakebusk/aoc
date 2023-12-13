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

  char line[8192];
  fgets(line, 8192, fp);
  fclose(fp);

  int charCount = strlen(line) - 1;

  int floor = 0;
  int part2 = 0;
  for (int c = 0; c < charCount; c++) {
    floor += line[c] == '(' ? 1 : -1;
    if (!part2 && floor == -1) part2 = c + 1;
  }

  printf("Part 1: %d\n", floor);
  printf("Part 2: %d\n", part2);
}
