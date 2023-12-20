#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int run(int sourceCode[512], int noun, int verb) {
  int program[512];
  memcpy(program, sourceCode, sizeof(int) * 512);
  program[1] = noun;
  program[2] = verb;

  int programCounter = 0;
  int halt = 0;
  while (!halt) {
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

    programCounter += 4;
  }

  return program[0];
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

  char line[512];
  fgets(line, 512, fp);
  fclose(fp);

  int sourceCode[512] = {0};
  int programLength = 0;

  char *token = 0;
  while ((token = strtok(token ? NULL : line, ","))) {
    sourceCode[programLength++] = atoi(token);
  }

  printf("Part 1: %d\n", run(sourceCode, 12, 2));

  int part2 = 0;
  for (int noun = 0; noun < 100 && !part2; noun++) {
    for (int verb = 0; verb < 100; verb++) {
      if (run(sourceCode, noun, verb) == 19690720) {
        part2 = 100 * noun + verb;
        break;
      }
    }
  }
  printf("Part 2: %d\n", part2);
}
