#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isWithinRange(long long ranges[200][2], int rangeCount, long long check) {
  for (int r = 0; r < rangeCount; r++) {
    if (check >= ranges[r][0] && check <= ranges[r][1]) return 1;
  }
  return 0;
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

  int part1 = 0;

  long long ranges[200][2];
  int rangeCount = 0;

  char line[64];
  int mode = 1;
  while(fgets(line, 64, fp)) {
    if (line[0] == '\n') {
      mode = 2;
      continue;
    }

    if (mode == 1) {
      sscanf(line, "%lld-%lld", &ranges[rangeCount][0], &ranges[rangeCount][1]);
      rangeCount++;
    } else {
      long long check = atoll(line);
      part1 += isWithinRange(ranges, rangeCount, check);
    }
  }
  fclose(fp);

  printf("Part 1: %d\n", part1);
}
