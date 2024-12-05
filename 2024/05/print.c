#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int rules[1200][2];
int ruleCount = 0;

int compare(const void* a, const void* b) {
  int val1 = *(int*)a;
  int val2 = *(int*)b;
  
  for (int i = 0; i < ruleCount; i++) {
    if (val1 == rules[i][0] && val2 == rules[i][1]) {
      return -1;
    } else if (val1 == rules[i][1] && val2 == rules[i][0]) {
      return 1;
    }
  }
  return 0;
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


  int updates[256][32];
  memset(updates, 0, sizeof(int) * 256 * 32);
  int updateCount = 0;

  int mode = 0;
  char line[512];
  while(fgets(line, 512, fp)) {
    if (strlen(line) == 1) {
      mode = 1;
      continue;
    }

    if (mode == 0) {
      int a, b;
      sscanf(line, "%d|%d", &a, &b);
      rules[ruleCount][0] = a;
      rules[ruleCount][1] = b;
      ruleCount++;
    } else {
      int length = 0;
      char *token = 0;
      while ((token = strtok(token ? NULL : line, ","))) {
        updates[updateCount][length++] = atoi(token);
      }
      updateCount++;
    }
  }
  fclose(fp);

  int part1 = 0;
  int part2 = 0;
  
  for (int u = 0; u < updateCount; u++) {
    int valid = 1;
    int p = -1;
    while (updates[u][++p] > 0) {
      int page = updates[u][p];
      for (int i = p - 1; i >= 0; i--) {
        int testPage = updates[u][i];

        for (int r = 0; r < ruleCount; r++) {
          if (page == rules[r][0] && testPage == rules[r][1]) {
            valid = 0;
          }
        }
      }
    }
    if (valid) {
      part1 += updates[u][p / 2];
    } else {
      qsort(updates[u], p, sizeof(int), compare);
      part2 += updates[u][p / 2];
    }
  }

  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}
