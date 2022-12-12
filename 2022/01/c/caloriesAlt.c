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


  int max = 0;
  int current = 0;

  char line[10];
  while(fgets(line, 10, fp)) {
    if (strcmp(line, "\n") == 0) {
      if (current > max) {
        max = current; 
      }
      current = 0;
    } else {
      int parsed = atoi(line);
      current += parsed;
    }
  }
  if (current > max) max = current;

  printf("Max: %d\n", max);

  fclose(fp);
}