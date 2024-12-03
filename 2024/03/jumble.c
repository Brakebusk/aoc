#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char mulPattern[] = "mul(";
const char doPattern[] = "do()";
const char dontPattern[] = "don't()";

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
  int part2 = 0;
  
  int enabled = 1;
  
  char line[4096];
  while(fgets(line, 4096, fp)) {
    int length = strlen(line);
    
    for (int c = 0; c < length; c++) {
      if (strncmp(&line[c], mulPattern, 4) == 0) {
        int a = -1, b = -1;
        char end;
        sscanf(&line[c], "mul(%d,%d%c", &a, &b, &end);
        if (a <= 0 || b <= 0 || a > 999 || b > 999 || end != ')') {
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
          int product = a * b;
          part1 += product;
          if (enabled) part2 += product;
        }
      } else if (strncmp(&line[c], doPattern, 4) == 0) {
        enabled = 1;
      } else if (strncmp(&line[c], dontPattern, 7) == 0) {
        enabled = 0;
      }
    }
  }
  fclose(fp);

  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}
