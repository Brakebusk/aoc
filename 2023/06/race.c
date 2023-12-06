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

  char trueTimeBuffer[32] = {0};
  char trueDistanceBuffer[32] = {0};

  char line[128];
  while(fgets(line, 128, fp)) {
    char *token;
    token = strtok(line, ":");
    int isTime = strcmp(token, "Time") == 0;

    int raceIndex = 0;
    while ((token = strtok(NULL, " "))) {
      if (isTime) {
        memcpy(&trueTimeBuffer[strlen(trueTimeBuffer)], token, strlen(token));
        races[raceCount++] = (struct race) {
          .time = atoi(token)
        };
      } else {
        memcpy(&trueDistanceBuffer[strlen(trueDistanceBuffer)], token, strlen(token));
        races[raceIndex++].recordDistance = atoi(token);
      }
    }
  }
  fclose(fp);

  long long trueTime = strtoll(trueTimeBuffer, NULL, 10);
  long long trueDistance = strtoll(trueDistanceBuffer, NULL, 10);

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

  int part2 = 0;

  for (long long speed = 0; speed < trueTime; speed++) {
    long long distanceCovered = speed * (trueTime - speed);
    if (distanceCovered > trueDistance) part2++;
  }

  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}

