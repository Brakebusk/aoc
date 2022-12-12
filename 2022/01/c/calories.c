#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct elf {
  int calories;
};
struct elf** elves;

int main(int argc, char *argv[]) {
  elves = malloc(500 * sizeof(struct elf*));

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

  struct elf *new = (struct elf*) malloc(sizeof(struct elf));
  new -> calories = 0;
  elves[0] = new;


  int elfIndex = 0;
  char line[10];
  while(fgets(line, 10, fp)) {
    if (strcmp(line, "\n") == 0) {
      struct elf *new = (struct elf*) malloc(sizeof(struct elf));
      new -> calories = 0;
      elves[++elfIndex] = new;
    } else {
      int parsed = atoi(line);
      struct elf *selected = elves[elfIndex];
      selected -> calories += parsed;
    }
  }

  int max;
  for (int i = 0; i <= elfIndex; i++) {
    struct elf *current = elves[i];
    int value = current -> calories;
    if (value > max) max = value;
  }

  printf("Max: %d\n", max);

  free(elves);
  fclose(fp);
}