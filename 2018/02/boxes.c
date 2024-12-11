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

  int two = 0;
  int three = 0;

  char line[32];
  int length = 0;
  while(fgets(line, 32, fp)) {
    if (length == 0) length = strlen(line) - 1;
    
    char counts[26] = {0};
    for (int i = 0; i < length; i++) counts[line[i] - 'a']++;
    for (int i = 0; i < 26; i++) {
      if (counts[i] == 2) {
        two++;
        break;
      }
    }
    for (int i = 0; i < 26; i++) {
      if (counts[i] == 3) {
        three++;
        break;
      }
    }
  }
  fclose(fp);

  printf("Part 1: %d\n", two * three);
}
