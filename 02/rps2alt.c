#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Opponent:
  A => Rock
  B => Paper
  C => Scissors

  You:
  X => Must lose = 0
  Y => Must draw = 3
  Z => Must win  = 6

  Rock     = 1 point
  Paper    = 2 points
  Scissors = 3 points
*/

int decissionMatrix[3][3] = {
  {3, 4, 8}, // Opponent A
  {1, 5, 9}, // Opponent B
  {2, 6, 7}, // Opponent C
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

  int score = 0;

  char line[5];
  while(fgets(line, 5, fp)) {
    char opponent, response;
    sscanf(line, "%c %c", &opponent, &response);
    score += decissionMatrix[opponent - 65][response - 88];
  }

  fclose(fp);

  printf("Score: %d\n", score);
}