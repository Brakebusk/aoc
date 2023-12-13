#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct board {
  int grid[5][5];
};

void printBoard(struct board b) {
  for (int r = 0; r < 5; r++) {
    for (int c = 0; c < 5; c++) {
      printf("%d ", b.grid[r][c]);
    }
    printf("\n");
  }
}

void mark(struct board *b, int winningNumber) {
  for (int r = 0; r < 5; r++) {
    for (int c = 0; c < 5; c++) {
      if (b->grid[r][c] == winningNumber) {
        b->grid[r][c] = -1;
      }
    }
  }
}

int isWinning(struct board b) {
  for (int r = 0; r < 5; r++) {
    int winningRow = 1;
    for (int c = 0; c < 5; c++) {
      if (b.grid[r][c] >= 0) {
        winningRow = 0;
        break;
      }
    }
    if (winningRow) return 1;
  }

  for (int c = 0; c < 5; c++) {
    int winningColumn = 1;
    for (int r = 0; r < 5; r++) {
      if (b.grid[r][c] >= 0) {
        winningColumn = 0;
        break;
      }
    }
    if (winningColumn) return 1;
  }

  return 0;
}

int calculateScore(struct board b, int winningNumber) {
  int score = 0;
  for (int r = 0; r < 5; r++) {
    for (int c = 0; c < 5; c++) {
      int value = b.grid[r][c];
      if (value > 0) score += value;
    }
  }
  return score * winningNumber;
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

  int winningNumbers[512];
  int winningNumberCount = 0;

  struct board boards[128];
  int boardCount = 0;

  char line[512];
  int boardRow = -1;
  while(fgets(line, 512, fp)) {
    if (!winningNumberCount) {
      char *token = strtok(line, ",");
      winningNumbers[winningNumberCount++] = atoi(token);
      while ((token = strtok(NULL, ","))) {
        winningNumbers[winningNumberCount++] = atoi(token);
      }
    } else {
      if (boardRow >= 0) {
        if (boardRow == 0) boardCount++;
        char *token = strtok(line, " ");
        int c = 0;
        boards[boardCount-1].grid[boardRow][c++] = atoi(token);
        while ((token = strtok(NULL, " "))) {
          boards[boardCount-1].grid[boardRow][c++] = atoi(token);
        }
      }
      boardRow++;
      if (boardRow == 5) boardRow = -1;
    }
  }
  fclose(fp);

  for (int w = 0; w < winningNumberCount; w++) {
    int winning = winningNumbers[w];
    for (int b = 0; b < boardCount; b++) {
      mark(&boards[b], winning);
    }

    for (int b = 0; b < boardCount; b++) {
      if (isWinning(boards[b])) {
        printf("Part 1: %d\n", calculateScore(boards[b], winning));
        w = winningNumberCount;
        break;
      }
    }
  }
}
