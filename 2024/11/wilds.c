#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STONES 250000

void printStones(long long stones[MAX_STONES], int count) {
  for (int i = 0; i < count; i++) {
    printf("%lld ", stones[i]);
  }
  printf("\n");
}

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

  char line[64];
  fgets(line, 64, fp);
  fclose(fp);

  long long stones[MAX_STONES];
  int count = 0;

  char *token;
  while ((token = strtok(token ? NULL : line, " "))) {
    stones[count++] = atoll(token);
  }
  
  for (int i = 0; i < 25; i++) {
    for (int s = 0; s < count; s++) {
      char str[24] = {0};
      sprintf(str, "%lld", stones[s]);
      int len;
      
      if (stones[s] == 0) {
        stones[s] = 1;
      } else if ((len = strlen(str)) % 2 == 0) {
        char half1[12] = {0}, half2[12] = {0};
        strncpy(half1, str, len / 2);
        strncpy(half2, &str[len/2], len / 2);
        stones[s] = atoll(half1);
        for (int shift = count-1; shift > s; shift--) {
          stones[shift + 1] = stones[shift];
        }
        stones[s+1] = atoll(half2);
        count++;
        if (count > MAX_STONES) {
          printf("[ERROR] Too many stones");
          exit(EXIT_FAILURE);
        }
        s++;
      } else  {
       stones[s] *= 2024; 
      }
    }
  }

  printf("Part 1: %d\n", count);
}
