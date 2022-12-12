#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Forest {
  int width;
  char *trees;
};

struct Forest* createForest(int forestWidth) {
  struct Forest* f = (struct Forest*) malloc(sizeof(struct Forest));
  f->width = forestWidth;
  f->trees = malloc(forestWidth * forestWidth * sizeof(char));
  return f;
}

char get(struct Forest *forest, int row, int col) {
  return forest->trees[row * forest->width + col];
}

void set(struct Forest *forest, int row, int col, char value) {
  forest->trees[row * forest->width + col] = value;
}

int isVisible(struct Forest *forest, int row, int col) {
  char height = get(forest, row, col);
  
  // Forward
  for (int c = 0; c <= col; c++) {
    if (c == col) return 1;
    if (get(forest, row, c) >= height) break;
  }

  // Backward
  for (int c = forest->width - 1; c >= col; c--) {
    if (c == col) return 1;
    if (get(forest, row, c) >= height) break;
  }

  // Down
  for (int r = 0; r <= row; r++) {
    if (r == row) return 1;
    if (get(forest, r, col) >= height) break;
  } 

  // Up
  for (int r = forest->width - 1; r >= row; r--) {
    if (r == row) return 1;
    if (get(forest, r, col) >= height) break;
  }

  return 0;
}

int calculateScenicScore(struct Forest *forest, int row, int col) {
  char height = get(forest, row, col);
  
  // Left
  int left = 0;
  for (int c = col - 1; c >= 0; c--) {
    left++;
    if (get(forest, row, c) >= height) break;
  }

  // Right
  int right = 0;
  for (int c = col + 1; c < forest->width; c++) {
    right ++;
    if (get(forest, row, c) >= height) break;
  }

  // Up
  int up = 0;
  for (int r = row - 1; r >= 0; r--) {
    up++;
    if (get(forest, r, col) >= height) break;
  } 

  // Down
  int down = 0;
  for (int r = row + 1; r < forest->width; r++) {
    down++;
    if (get(forest, r, col) >= height) break;
  }

  return left * right * up * down;
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

  struct Forest *forest;

  char line[120];
  int lineIndex = 0;
  while(fgets(line, 120, fp)) {
    int lineLength = strlen(line) - 1;
    if (lineIndex == 0) {
      forest = createForest(lineLength);
    }

    for (int i = 0; i < lineLength; i++) {
      set(forest, lineIndex, i, line[i] - 48);
    }

    lineIndex++;
  }

  int visible = 0;
  int mostScenic = 0;

  for (int row = 0; row < forest->width; row++){
    for (int col = 0; col < forest->width; col++) {
      visible += isVisible(forest, row, col);
      int score;
      if ((score = calculateScenicScore(forest, row, col)) > mostScenic) mostScenic = score;
    }
  }

  printf("Part 1: %d\n", visible);
  printf("Part 2: %d\n", mostScenic);

  fclose(fp);
}