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

  int copies[256] = {0};

  char line[256];
  while(fgets(line, 256, fp)) {
    int winCount = 0;
    int points = 0;
    int winning[100] = {0};
    
    char *token;
    token = strtok(line, ":");
    int cardId;
    sscanf(token, "Card %d", &cardId);
    copies[cardId]++;
    token = strtok(NULL, "|");

    // Mark winning numbers
    char* rest = token;
    char* segment;
    while ((segment = strtok_r(rest, " ", &rest))) {
      int value = atoi(segment);
      winning[value] = 1;
    }

    token = strtok(NULL, "\n");
    rest = token;
    while ((segment = strtok_r(rest, " ", &rest))) {
      int value = atoi(segment);
      if (winning[value]) {
        winCount++;
        if (points) {
          points *= 2;
        } else {
          points = 1;
        }
      }
    }
    part1 += points;

    for (int i = 0; i < winCount; i++) {
      copies[1 + cardId + i] += copies[cardId];
    }
  }
  fclose(fp);

  int part2 = 0;
  for (int i = 0; i < sizeof(copies) / sizeof(int); i++) part2 += copies[i];

  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}

