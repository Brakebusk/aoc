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

  int depths[2048];
  int depthCount = 0;

  char line[512];
  while(fgets(line, 512, fp)) {
    depths[depthCount++] = atoi(line);
  }
  fclose(fp);

  int part1 = 0;

  int prev = depths[0];
  for (int d = 1; d < depthCount; d++) {
    if (depths[d] > prev) {
      part1++;
    }
    prev = depths[d];
  }

  int part2 = 0;

  int slides[2048];
  int slideCount = 0;
  for (int d = 0; d < depthCount - 2; d++) {
    slides[slideCount++] = depths[d] + depths[d+1] + depths[d+2];
    if (d > 0 && slides[slideCount-1] > slides[slideCount-2]) part2++;
  }

  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}
