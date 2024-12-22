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

  long long part1 = 0;

  char line[16];
  while(fgets(line, 16, fp)) {
    long long secret = atoll(line);
    for (int i = 0; i < 2000; i++) {
      secret ^= secret * 64;
      secret %= 16777216;
      secret ^= secret / 32;
      secret %= 16777216;
      secret ^= secret * 2048,
      secret %= 16777216;
    }
    part1 += secret;
  }
  fclose(fp);

  printf("Part 1: %lld\n", part1);
}
