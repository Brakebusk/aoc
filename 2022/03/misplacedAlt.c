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

  int sum = 0;

  char line[50];
  while(fgets(line, 50, fp)) {
    char compartments[52];
    memset(compartments, 0, sizeof(char) * 52);

    char length = strlen(line);

    for (int i = 0; i < length; i++) {
        char value = line[i] - 39;
        if (value > 51) {
            value -= 58;
        }

        if (i >= length >> 1) {
            if (compartments[value]) {
                sum += value + 1;
                break;
            }
        } else {
            compartments[value] = 1;
        }
    }
  }

  fclose(fp);

  printf("part 1: %d\n", sum);
}