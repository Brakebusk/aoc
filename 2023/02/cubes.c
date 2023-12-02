#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int RED = 12;
const int GREEN = 13;
const int BLUE = 14;

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

  char line[200];
  while(fgets(line, 200, fp)) {
    int failed = 0;
    char *rest = line;
    char *lineSegment = strtok_r(rest, ":", &rest);
    int gameId;
    sscanf(lineSegment, "Game %d", &gameId);

    int minRed = 0;
    int minGreen = 0;
    int minBlue = 0;

    while ((lineSegment = strtok_r(rest, ";", &rest))) {
      char *segmentRest = lineSegment;
      char *token;
      while ((token = strtok_r(segmentRest, ",", &segmentRest))) {
        char color[6];
        int value;
        sscanf(token, " %d %s", &value, color);

        if (strcmp(color, "red") == 0) {
          if (value > minRed) {
            minRed = value;
          }
          if (value > RED) {
            failed = 1;
          }
        } else if (strcmp(color, "green") == 0) {
          if (value > minGreen) {
            minGreen = value;
          }
          if (value > GREEN) {
            failed = 1;
          }
        } else if (strcmp(color, "blue") == 0) {
          if (value > minBlue) {
            minBlue = value;
          }
          if (value > BLUE) {
            failed = 1;
          }
        } else {
          printf("ERROR: Unknown color: %s\n", color);
          exit(EXIT_FAILURE);
        }
      }
    }
    
    part2 += minRed * minGreen * minBlue;
    if (!failed) part1 += gameId;
  }

  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}

