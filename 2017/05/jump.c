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

  int memory[1200];
  int length = 0;

  char line[8];
  while(fgets(line, 8, fp)) {
    memory[length++] = atoi(line);
  }
  fclose(fp);

  int ip = 0;
  int steps = 0;
  while (ip >= 0 && ip < length) {
    memory[ip]++;
    ip += memory[ip]-1;
    steps++;
  }
  printf("Part 1: %d\n", steps);
}
