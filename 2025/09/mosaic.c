#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tile {
  int x, y;
};

long long getArea(struct tile a, struct tile b) {
  return (1 + llabs(a.x - b.x)) * (1 + llabs(a.y - b.y));
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

  struct tile tiles[512];
  int tc = 0;

  char line[16];
  while(fgets(line, 16, fp)) {
    sscanf(line, "%d,%d", &tiles[tc].x, &tiles[tc].y);
    tc++;
  }
  fclose(fp);

  long long part1 = 0;

  for (int a = 0; a < tc; a++) {
    for (int b = a + 1; b < tc; b++) {
      long long area = getArea(tiles[a], tiles[b]);
      if (area > part1) part1 = area;
    }
  }

  printf("Part 1: %lld\n", part1);
}
