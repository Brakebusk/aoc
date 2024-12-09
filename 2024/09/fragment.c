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

  int *result1 = malloc(sizeof(int) * LIM);
  int *result2 = malloc(sizeof(int) * LIM);
  int length = 0;

  int index = 0;
  for (int c = 0; c < strlen(line); c++) {
    char v = line[c] - '0';
    if (c % 2 == 0) {
      for (int i = 0; i < v; i++) result1[i+length] = index;
      index++;
    } else {
      for (int i = 0; i < v; i++) result1[i+length] = -1;
    }
    length += v;
    if (length >= LIM) {
      printf("[ERROR] Exceeded limit\n");
      exit(EXIT_FAILURE);
    }
  }
  free(line);
  memcpy(result2, result1, sizeof(int) * LIM);

  for (int i = length - 1; i >= 0; i--) {
    if (result1[i] > -1) {
      int place = -1;
      for (int j = 0; j < i; j++) {
        if (result1[j] == -1) {
          place = j;
          break;
        }
      }
      if (place > -1) {
        result1[place] = result1[i];
        result1[i] = -1;
      }
    }
  }

  for (int i = length - 1; i >= 0; i--) {
    if (result2[i] > -1) {
      int fileSize = 0;
      int v = result2[i];
      for (int j = i; j >= 0; j--) {
        if (result2[j] != v) break;
        fileSize++;
      }
      int place = -1;
      for (int j = 0; j < i; j++) {
        int found = 1;
        for (int o = 0; o < fileSize; o++) {
          if (result2[j+o] != -1) {
            found = 0;
            break;
          }
        }
        if (found) {
          place = j;
          break;
        }
      }
      if (place > -1) {
        for (int p = 0; p < fileSize; p++) {
          result2[i - p] = -1;
        }
        for (int p = place; p < place + fileSize; p++) {
          result2[p] = v;
        }
      }
      i -= fileSize - 1;
    }
  }
  
  long long part1 = 0;
  long long part2 = 0;
  for (int i = 0; i < length; i++) {
    if (result1[i] == -1) break;
    part1 += i * result1[i];
  }
  for (int i = 0; i < length; i++) {
    if (result2[i] > -1) {
      part2 += i * result2[i];
    }
  }
  printf("Part 1: %lld\n", part1);
  printf("Part 2: %lld\n", part2);

  free(result1);
  free(result2);
}
