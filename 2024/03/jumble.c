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

  int part1 = 0;
  
  char pattern[] = "mul(";
  char line[4096];
  while(fgets(line, 4096, fp)) {
    int c = 0;
    int length = strlen(line);
    while (c < length) {
      if (strncmp(&line[c], pattern, 4) == 0) {
        int a = -1, b = -1;
        char end;
        sscanf(&line[c], "mul(%d,%d%c", &a, &b, &end);
        if (a <= 0 || b <= 0 || a > 999 || b > 999 || end != ')') {
          c++;
          continue;
        }
        int valid = 1;
        int i = c;
        while (i++) {
          if (line[i] == ' ') {
            valid = 0;
            break;
          } else if (line[i] == ')') break;
        }
        if (valid) {
          part1 += a * b;
        }
      }
      c++;
    }
  }
  fclose(fp);

  printf("Part 1: %d\n", part1);
}
