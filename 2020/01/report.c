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

  int expenses[256];
  int expenseCount;

  char line[8];
  while(fgets(line, 8, fp)) {
    expenses[expenseCount++] = atoi(line);
  }
  fclose(fp);

  int part1 = 0;
  int part2 = 0;
  for (int a = 0; a < expenseCount - 1 && !(part1 && part2); a++) {
    for (int b = a + 1; b < expenseCount && !(part1 && part2); b++) {
      if (!part1 && expenses[a] + expenses[b] == 2020) {
        part1 = expenses[a] * expenses[b];
      }
      if (!part2) {
        for (int c = b + 1; c < expenseCount; c++) {
          if (expenses[a] + expenses[b] + expenses[c] == 2020) {
            part2 = expenses[a] * expenses[b] * expenses[c];
            break;
          }
        }
      }
    }
  }
  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}
