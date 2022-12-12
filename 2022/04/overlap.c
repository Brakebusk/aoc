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

  int overlapping = 0;


  char line[20];
  while(fgets(line, 20, fp)) {

    int e1Min, e1Max, e2Min, e2Max;
    sscanf(line, "%d-%d,%d-%d", &e1Min, &e1Max, &e2Min, &e2Max);

    if ((e1Min >= e2Min && e1Max <= e2Max) || (e2Min >= e1Min && e2Max <= e1Max)) {
        overlapping++;
    }
  }

  fclose(fp);

  printf("part 1: %d\n", overlapping);
}