#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long raise(long long x, long long y) {
  long long value = 1;
  for (long long i = 0; i < y; i++) {
    value *= x;
  }
  return value;
}

long long parseSnafu(char* string) {
  long long value = 0;
  int power = 0;
  for (int d = strlen(string)-2; d >= 0; d--) {
    int v;
    switch (string[d]) {
      case '2':
        v = 2;
        break;
      case '1':
        v = 1;
        break;
      case '0':
        v = 0;
        break;
      case '-':
        v = -1;
        break;
      case '=':
        v = -2;
        break;
      default:
        printf("Unknown character '%c'\n", string[d]);
        exit(EXIT_FAILURE);
    }
    value += v * raise(5, power++);
  }
  return value;
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

  long long part1 = 0;

  char line[128];
  while(fgets(line, 128, fp)) {
    part1 += parseSnafu(line);
  }
  fclose(fp);

  printf("Part 1: %lld\n", part1);
}

