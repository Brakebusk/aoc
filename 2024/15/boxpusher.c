#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

  char matrix[64][64];
  memset(matrix, 0, 64 * 64);
  int size = 0;

  char moves[22000] = {0};
  int moveCount = 0;

  char line[1024];
  int mode = 0;
  int lc = 0;
  int moveLineLength = 0;
  while(fgets(line, 1024, fp)) {
    if (size == 0) size = strlen(line) - 1;
    if (mode == 0) {
      if (strlen(line) > 2) {
        memcpy(matrix[lc++], line, size);
      } else mode = 1;
    } else {
      if (moveLineLength == 0) moveLineLength = strlen(line) - 1;
      memcpy(&moves[moveCount], line, moveLineLength);
      moveCount += moveLineLength;
    }
  }
  fclose(fp);

  int row = -1, col = -1;
  for (int r = 0; r < size && row == -1; r++) {
    for (int c = 0; c < size; c++) {
      if (matrix[r][c] == '@') {
        row = r;
        col = c;
        matrix[r][c] = '.';
        break;
      }
    }
  }

  for (int i = 0; i < moveCount; i++) {
    char move = moves[i];
    switch (move) {
      case '^':
        if (matrix[row-1][col] == '.') {
          row--;
        } else if (matrix[row-1][col] == 'O') {
          int found = -1;
          for (int r = row-1; r > 0; r--) {
            if (matrix[r][col] == '.') {
              found = r;
              break;
            } else if (matrix[r][col] == '#') break;
          }
          if (found > -1) {
            for (int shift = found; shift < row; shift++) {
              matrix[shift][col] = matrix[shift+1][col];
            }
            row--;
          }
        }
        break;
      case '>':
        if (matrix[row][col+1] == '.') {
          col++;
        } else if (matrix[row][col+1] == 'O') {
          int found = -1;
          for (int c = col+1; c < size - 1; c++) {
            if (matrix[row][c] == '.') {
              found = c;
              break;
            } else if (matrix[row][c] == '#') break;
          }
          if (found > -1) {
            for (int shift = found; shift > col; shift--) {
              matrix[row][shift] = matrix[row][shift-1];
            }
            col++;
          }
        }
        break;
      case 'v':
        if (matrix[row+1][col] == '.') {
          row++;
        } else if (matrix[row+1][col] == 'O') {
          int found = -1;
          for (int r = row+1; r < size - 1; r++) {
            if (matrix[r][col] == '.') {
              found = r;
              break;
            } else if (matrix[r][col] == '#') break;
          }
          if (found > -1) {
            for (int shift = found; shift > row; shift--) {
              matrix[shift][col] = matrix[shift-1][col];
            }
            row++;
          }
        }
        break;
      case '<':
        if (matrix[row][col-1] == '.') {
          col--;
        } else if (matrix[row][col-1] == 'O') {
          int found = -1;
          for (int c = col-1; c > 0; c--) {
            if (matrix[row][c] == '.') {
              found = c;
              break;
            } else if (matrix[row][c] == '#') break;
          }
          if (found > -1) {
            for (int shift = found; shift < col; shift++) {
              matrix[row][shift] = matrix[row][shift+1];
            }
            col--;
          }
        }
        break;
      default:
        printf("Unknown move: '%c'\n", move);
        exit(EXIT_FAILURE);
    }
  }

  int part1 = 0;
  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      if (matrix[r][c] == 'O') part1 += 100 * r + c;
    }
  }
  printf("Part 1: %d\n", part1);
}
