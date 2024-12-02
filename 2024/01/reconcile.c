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

  int length = 0;
  int list1[1024];
  int list2[1024];

  char line[32];
  while(fgets(line, 32, fp)) {
    sscanf(line, "%d   %d", &list1[length], &list2[length]);
    length++;
  }
  fclose(fp);

  qsort(list1, length, sizeof(int), compare);
  qsort(list2, length, sizeof(int), compare);

  int part1 = 0;
  int part2 = 0;
  for (int i = 0; i < length; i++) {
    part1 += abs(list1[i] - list2[i]);

    int occurances = 0;
    for (int k = 0; k < length; k++) {
      if (list1[i] == list2[k]) {
        occurances ++;
      } else if (list1[i] < list2[k]) break;
    }
    part2 += list1[i] * occurances;
  }

  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}
