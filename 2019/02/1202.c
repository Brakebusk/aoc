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

  char line[512];
  fgets(line, 512, fp);
  fclose(fp);

  int program[512] = {0};
  int programLength = 0;

  char *token = 0;
  while ((token = strtok(token ? NULL : line, ","))) {
    program[programLength++] = atoi(token);
  }

  program[1] = 12;
  program[2] = 2;

  int programCounter = -4;
  int halt = 0;
  while (!halt) {
    programCounter += 4;
    int opCode = program[programCounter];

    int p1, p2, p3;
    switch (opCode) {
      case 1:
        p1 = program[programCounter+1];
        p2 = program[programCounter+2];
        p3 = program[programCounter+3];
        program[p3] = program[p1] + program[p2];
        break;
      case 2:
        p1 = program[programCounter+1];
        p2 = program[programCounter+2];
        p3 = program[programCounter+3];
        program[p3] = program[p1] * program[p2];
        break;
      case 99:
        halt = 1;
        break;
      default:
        printf("Unknown opCode %d\n", opCode);
        exit(EXIT_FAILURE);
    }
  }

  printf("Part 1: %d\n", program[0]);
}
