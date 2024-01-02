#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CARD_COUNT 10

void deal(int *from, int *to) {
  for (int i = 0; i < CARD_COUNT; i++) {
    to[CARD_COUNT-1-i] = from[i];
    from[i] = -1;
  }
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
  while(fgets(line, 512, fp)) {

  }
  fclose(fp);
}
