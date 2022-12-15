#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct Buffer {
  int depths[5];
  int top;
};

void add(struct Buffer* buffer, int value) {
  buffer->top = (buffer->top + 1) % 5;
  buffer->depths[buffer->top] = value;
}

int getPrev(struct Buffer* buffer, int offset) {
  return buffer->depths[(buffer->top - offset) % 5];
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
  int part2 = 0;

  struct Buffer prev;
  prev.top = 0;
  for (int i = 0; i < 3; i++) prev.depths[i] = INT_MAX;

  char line[10];
  int lineIndex = 0;
  while(fgets(line, 10, fp)) {
    int depth;
    sscanf(line, "%d", &depth);

    if (depth > getPrev(&prev, 0)) part1++;

    int common = getPrev(&prev, 0) + getPrev(&prev, 1);
    printf("%d vs %d = %d\n", common + depth, common + getPrev(&prev, 2), common + depth > common + getPrev(&prev, 2));
    if (lineIndex++ > 3 && common + depth > common + getPrev(&prev, 2)) part2++;

    add(&prev, depth);
  }

  fclose(fp);

  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}
