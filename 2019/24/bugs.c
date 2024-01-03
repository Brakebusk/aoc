#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HISTORY 1024

int max(int a, int b) {
  return a > b ? a : b;
}

int min(int a, int b) {
  return a < b ? a : b;
}

void printGrid(char grid[5][5]) {
  for (int r = 0; r < 5; r++) {
    for (int c = 0; c < 5; c++) {
      printf("%c", grid[r][c]);
    }
    printf("\n");
  }
  printf("\n");
}

int isInHistory(char grid[5][5], char history[HISTORY][5][5], int historyLength) {
  for (int h = 0; h < historyLength; h++) {
    int repeating = 1;
    for (int r = 0; r < 5 && repeating; r++) {
      for (int c = 0; c < 5; c++) {
        if (grid[r][c] != history[h][r][c]) {
          repeating = 0;
          break;
        }
      }
    }
    if (repeating) return 1;
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

  char grid[5][5];

  char line[8];
  int lineCounter = 0;
  while(fgets(line, 8, fp)) {
    memcpy(grid[lineCounter++], line, sizeof(char) * 5);
  }
  fclose(fp);

  char history[HISTORY][5][5];
  int historyLength = 0;
  for (int r = 0; r < 5; r++) {
    for (int c = 0; c < 5; c++) {
      history[0][r][c] = grid[r][c];
    }
  }
  historyLength++;

  int repeating = 0;
  while (!repeating) {
    char newGrid[5][5];

    for (int r = 0; r < 5; r++) {
      for (int c = 0; c < 5; c++) {
        int aroundCount = 0;
        for (int tr = max(0, r - 1); tr <= min(4, r + 1); tr++) {
          for (int tc = max(0, c - 1); tc <= min(4, c + 1); tc++) {
            if (((tr == r && tc != c) || (tc == c && tr != r)) && grid[tr][tc] == '#') aroundCount++; 
          }
        }
        if (grid[r][c] == '.') {
          newGrid[r][c] = (aroundCount == 1 || aroundCount == 2) ? '#' : '.'; 
        } else if (grid[r][c] == '#') {
          newGrid[r][c] = aroundCount == 1 ? '#' : '.';
        } else {
          printf("Unknown tile '%c'\n", grid[r][c]);
          exit(EXIT_FAILURE);
        }
      }
    }

    repeating = isInHistory(newGrid, history, historyLength);

    for (int r = 0; r < 5; r++) {
      for (int c = 0; c < 5; c++) {
        history[historyLength][r][c] = newGrid[r][c];
        grid[r][c] = newGrid[r][c];
      }
    }
    historyLength++;
  }

  int rating = 0;
  int factor = 1;
  for (int r = 0; r < 5; r++) {
    for (int c = 0; c < 5; c++) {
      if (grid[r][c] == '#') rating += factor;
      factor *= 2;
    }
  }

  printf("Part 1: %d\n", rating);
}
