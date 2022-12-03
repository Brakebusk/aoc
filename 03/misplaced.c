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
    char compartments[2][52];
    memset(compartments, 0, sizeof(char) * 2 * 52);

    char length = strlen(line);

    for (int i = 0; i < length; i++) {
        char value = line[i];
        if (value > 90) {
          value -= 97;
        } else {
          value -= 39;
        }

        compartments[i >= (length >> 1)][value] = 1;
    }

    for (int i = 0; i < 52; i++) {
        if (compartments[0][i] && compartments[1][i]) {
            sum += i + 1;
            break;
        }
    }
  }

  fclose(fp);

  printf("part 1: %d\n", sum);
}