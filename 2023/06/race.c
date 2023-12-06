#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct race {
  int time,
      recordDistance;
};

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

  struct race races[8];
  int raceCount = 0;

  char line[128];
  while(fgets(line, 128, fp)) {
    char *token;
    token = strtok(line, ":");
    int isTime = strcmp(token, "Time") == 0;

    int raceIndex = 0;
    while ((token = strtok(NULL, " "))) {
      if (isTime) {
        races[raceCount++] = (struct race) {
          .time = atoi(token)
        };
      } else {
        races[raceIndex++].recordDistance = atoi(token);
      }
    }
  }
  fclose(fp);

  int part1 = 1;

  for (int i = 0; i < raceCount; i++) {
    int time = races[i].time;
    int recordDistance = races[i].recordDistance;
    int areRecordBreaking = 0;

    for (int speed = 0; speed < time; speed++) {
      int distanceCovered = speed * (time - speed);
      if (distanceCovered > recordDistance) areRecordBreaking++;
    }

    part1 *= areRecordBreaking;
  }

  printf("Part 1: %d\n", part1);
}

