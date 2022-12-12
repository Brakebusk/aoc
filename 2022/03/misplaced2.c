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

  char group[3][52];
  memset(group, 0, sizeof(char) * 3 * 52);
  char elf = 0;

  char line[50];
  while(fgets(line, 50, fp)) {

    char length = strlen(line);
    for (int i = 0; i < length; i++) {
        char value = line[i] - 39;
        if (value > 51) {
            value -= 58;
        }

        group[elf][value] = 1;
    }

    if (elf++ == 2) {
        for (int i = 0; i < 52; i++) {
            if (group[0][i] && group[1][i] && group[2][i]) {
                sum += i + 1;
                break;
            }
        }
        elf = 0;
        memset(group, 0, sizeof(char) * 3 * 52);  
    }

  }

  fclose(fp);

  printf("part 2: %d\n", sum);
}