#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Opponent:
  A => Rock
  B => Paper
  C => Scissors

  You:
  X => Rock     = 1 point
  Y => Paper    = 2 points
  Z => Scissors = 3 points

  Loss  = 0
  Draw  = 3
  Win   = 6
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
      case 'X':
        score += 1;
        if (opponent == 'C') {
          score += 6;
        } else if (opponent == 'A') {
          score += 3;
        }
        break;
      case 'Y':
        score += 2;
        if (opponent == 'A') {
          score += 6;
        } else if (opponent == 'B') {
          score += 3;
        }
        break;
      case 'Z':
        score += 3;
        if (opponent == 'B') {
          score += 6;
        } else if (opponent == 'C') {
          score += 3;
        }
        break;
    }
  }

  fclose(fp);

  printf("Score: %d", score);
}