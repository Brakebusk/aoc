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


  int elves[300];
  memset(elves, 0, sizeof(elves));
  int current = 0;

  char line[10];
  while(fgets(line, 10, fp)) {
    if (strcmp(line, "\n") == 0) {
      current++;
    } else {
      int parsed = atoi(line);
      elves[current] += parsed;
    }
  }
  fclose(fp);

  int max1 = 0;
  int max2 = 0;
  int max3 = 0;

  for (int i = 0; i < 300; i++) {
    int v = elves[i];
    if (v > max1) {
      max3 = max2;
      max2 = max1;
      max1 = v;
    } else if (v > max2) {
      max3 = max2;
      max2 = v;
    } else if (v > max3) {
      max3 = v;
    }
  }

  printf("%d + %d + %d = %d", max1, max2, max3, max1 + max2 + max3);
}