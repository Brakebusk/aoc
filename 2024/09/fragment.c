#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIM 100000

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

  char *line = malloc(sizeof(char) * 21000);
  fgets(line, 21000, fp);
  fclose(fp);

  int *result = malloc(sizeof(int) * LIM);
  int length = 0;

  int index = 0;
  for (int c = 0; c < strlen(line); c++) {
    char v = line[c] - '0';
    if (c % 2 == 0) {
      for (int i = 0; i < v; i++) result[i+length] = index;
      index++;
    } else {
      for (int i = 0; i < v; i++) result[i+length] = -1;
    }
    length += v;
    if (length >= LIM) {
      printf("[ERROR] Exceeded limit\n");
      exit(EXIT_FAILURE);
    }
  }
  free(line);

  for (int i = length - 1; i >= 0; i--) {
    if (result[i] > -1) {
      int place = -1;
      for (int j = 0; j < i; j++) {
        if (result[j] == -1) {
          place = j;
          break;
        }
      }
      if (place > -1) {
        result[place] = result[i];
        result[i] = -1;
      }
    }
  }
  
  long long part1 = 0;
  for (int i = 0; i < length; i++) {
    if (result[i] == -1) break;
    part1 += i * result[i];
  }
  printf("Part 1: %lld\n", part1);

  free(result);
}
