#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Opponent:
  A => Rock
  B => Paper
  C => Scissors

  You:
  X => Rock     = 1
  Y => Paper    = 2
  Z => Scissors = 3

  Loss  = 0
  Draw  = 3
  Win   = 6
*/

int scoreMatrix[3][3] = {
  {4, 8, 3}, // A -> {X, Y, Z}
  {1, 5, 9}, // B -> {X, Y, Z}
  {7, 2, 6}, // C -> {X, Y, Z}
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
    score += scoreMatrix[opponent - 65][response - 88];
  }

  fclose(fp);

  printf("Score: %d\n", score);
}