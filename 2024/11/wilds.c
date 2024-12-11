#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>

long long countStones(long long value, int remainingIters) {
  if (remainingIters == 0) return 1;
  
  ENTRY entry;
  entry.key = malloc(30);
  sprintf(entry.key, "%lld-%d", value, remainingIters);

  ENTRY *found = hsearch(entry, FIND);
  if (found != NULL) {
    free(entry.key);
    return *(long long *)found->data;
  }

  char str[24] = {0};
  sprintf(str, "%lld", value);
  int len;
  
  long long result;
  if (value == 0) {
    result = countStones(1, remainingIters - 1);
  } else if ((len = strlen(str)) % 2 == 0) {
    char half1[12] = {0}, half2[12] = {0};
    strncpy(half1, str, len / 2);
    strncpy(half2, &str[len/2], len / 2);
    
    result = countStones(atoll(half1), remainingIters - 1) + countStones(atoll(half2), remainingIters - 1);
  } else  {
    result = countStones(value * 2024, remainingIters - 1);
  }

  entry.data = malloc(sizeof(long long));
  *(long long *)entry.data = result;
  hsearch(entry, ENTER);

  return result;
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

  long long stones[10];
  int count = 0;

  char *token;
  while ((token = strtok(token ? NULL : line, " "))) {
    stones[count++] = atoll(token);
  }

  hcreate(500000);

  long long part1 = 0;
  long long part2 = 0;
  for (int s = 0; s < count; s++) {
    part1 += countStones(stones[s], 25);
    part2 += countStones(stones[s], 75);
  }
  printf("Part 1: %lld\n", part1);
  printf("Part 2: %lld\n", part2);
  
  hdestroy();
}
