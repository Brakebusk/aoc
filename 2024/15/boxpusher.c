#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -1 = up, 1 = down
int moveWideBoxes(char matrix[64][128], int width, int height, int row, int anyCol, int direction, int doIt) {
  int col = anyCol;
  if (matrix[row][col] == ']') col -= 1;
  
  if (matrix[row+direction][col] == '#' || matrix[row+direction][col+1] == '#') return 0;
  
  
  int leftCanMove = 0, rightCanMove = 0;
  if (matrix[row+direction][col] == '.') {
    leftCanMove = 1;
  } else leftCanMove = moveWideBoxes(matrix, width, height, row+direction, col, direction, doIt);
  if (matrix[row+direction][col+1] == '.') {
    rightCanMove = 1;
  } else rightCanMove = moveWideBoxes(matrix, width, height, row+direction, col+1, direction, doIt);

  if (doIt) {
    matrix[row+direction][col] = matrix[row][col];
    matrix[row+direction][col+1] = matrix[row][col+1];
    matrix[row][col] = '.';
    matrix[row][col+1] = '.';
  }

  return leftCanMove && rightCanMove;
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

  char matrix[64][64];
  char wideMatrix[64][128];
  memset(matrix, 0, 64 * 64);
  memset(wideMatrix, 0, 64 * 128);
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

  int width = size * 2;
  int height = size;
  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      switch(matrix[r][c]) {
        case '#':
          wideMatrix[r][2*c] = '#';
          wideMatrix[r][2*c+1] = '#';
          break;
        case 'O':
          wideMatrix[r][2*c] = '[';
          wideMatrix[r][2*c+1] = ']';
          break;
        case '.':
          wideMatrix[r][2*c] = '.';
          wideMatrix[r][2*c+1] = '.';
          break;
        case '@':
          wideMatrix[r][2*c] = '@';
          wideMatrix[r][2*c+1] = '.';
          break;
        default:
          printf("Unknown tile: '%c'\n", matrix[r][c]);
          exit(EXIT_FAILURE);
      }
    }
  }


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


  for (int r = 0; r < height; r++) {
    for (int c = 0; c < width; c++) {
      if (wideMatrix[r][c] == '@') {
        row = r;
        col = c;
        wideMatrix[r][c] = '.';
        break;
      }
    }
  }

  for (int i = 0; i < moveCount; i++) {
    char move = moves[i];
    switch (move) {
      case '^':
        if (wideMatrix[row-1][col] == '.') {
          row--;
        } else if (wideMatrix[row-1][col] == '[' || wideMatrix[row-1][col] == ']') {
          if (moveWideBoxes(wideMatrix, width, height, row-1, col, -1, 0)) {
            moveWideBoxes(wideMatrix, width, height, row-1, col, -1, 1);
            row--;
          }
        }
        break;
      case '>':
        if (wideMatrix[row][col+1] == '.') {
          col++;
        } else if (wideMatrix[row][col+1] == '[') {
          int found = -1;
          for (int c = col+1; c < width - 1; c++) {
            if (wideMatrix[row][c] == '.') {
              found = c;
              break;
            } else if (wideMatrix[row][c] == '#') break;
          }
          if (found > -1) {
            for (int shift = found; shift > col; shift--) {
              wideMatrix[row][shift] = wideMatrix[row][shift-1];
            }
            col++;
          }
        }
        break;
      case 'v':
        if (wideMatrix[row+1][col] == '.') {
          row++;
        } else if (wideMatrix[row+1][col] == '[' || wideMatrix[row+1][col] == ']') {
          if (moveWideBoxes(wideMatrix, width, height, row+1, col, 1, 0)) {
            moveWideBoxes(wideMatrix, width, height, row+1, col, 1, 1);
            row++;
          }
        }
        break;
      case '<':
        if (wideMatrix[row][col-1] == '.') {
          col--;
        } else if (wideMatrix[row][col-1] == ']') {
          int found = -1;
          for (int c = col-1; c > 0; c--) {
            if (wideMatrix[row][c] == '.') {
              found = c;
              break;
            } else if (wideMatrix[row][c] == '#') break;
          }
          if (found > -1) {
            for (int shift = found; shift < col; shift++) {
              wideMatrix[row][shift] = wideMatrix[row][shift+1];
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
  int part2 = 0;
  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      if (matrix[r][c] == 'O') part1 += 100 * r + c;
    }
  }
  for (int r = 0; r < height; r++) {
    for (int c = 0; c < width; c++) {
      if (wideMatrix[r][c] == '[') part2 += 100 * r + c;
    }
  }
  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}
