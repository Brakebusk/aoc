#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare(const void* a, const void* b) {
   return (*(int*)a - *(int*)b);
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

  char line[20];
  while(fgets(line, 20, fp)) {
    int values[3];
    sscanf(line, "%d %d %d", &values[0], &values[1], &values[2]);
    qsort(values, 3, sizeof(int), compare);
    if (values[0] + values[1] > values[2]) part1++;
  }
  fclose(fp);

  printf("Part 1: %d\n", part1);
}
