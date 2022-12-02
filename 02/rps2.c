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

  Rock     = 1
  Paper    = 2
  Scissors = 3
*/

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

    switch (response) {
      case 'X': // Lose
        if (opponent == 'A') {
          score += 3;
        } else if (opponent == 'B') {
          score += 1;
        } else if (opponent == 'C') {
          score += 2;
        }
        break;
      case 'Y': // Draw
        score += 3;
        if (opponent == 'A') {
          score += 1;
        } else if (opponent == 'B') {
          score += 2;
        } else if (opponent == 'C') {
          score += 3;
        }
        break;
      case 'Z': // Win
        score += 6;
        if (opponent == 'A') {
          score += 2;
        } else if (opponent == 'B') {
          score += 3;
        } else if (opponent == 'C') {
          score += 1;
        }
        break;
    }
  }

  fclose(fp);

  printf("Score: %d\n", score);
}