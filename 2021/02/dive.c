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

  int x = 0, depth = 0;

  char line[32];
  while(fgets(line, 32, fp)) {
    char instruction[32];
    int magnitude;
    sscanf(line, "%s %d", instruction, &magnitude);

    if (strcmp(instruction, "forward") == 0) {
      x += magnitude;
    } else if (strcmp(instruction, "down") == 0) {
      depth += magnitude;
    } else if (strcmp(instruction, "up") == 0) {
      depth -= magnitude;
    } else {
      printf("Unknown command '%s'\n", instruction);
      exit(EXIT_FAILURE);
    }
  }
  fclose(fp);

  printf("Part 1: %d\n", x * depth);
}
