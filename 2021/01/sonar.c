#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

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

  int increasing = 0;
  int prev = INT_MAX;
  
  char line[10];
  while(fgets(line, 10, fp)) {
    int depth;
    sscanf(line, "%d", &depth);

    if (depth > prev) increasing++;
    prev = depth;
  }

  printf("Part 1: %d\n", increasing);
}
