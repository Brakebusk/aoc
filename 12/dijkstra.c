#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Board {
  int width;
  int height;
  char *squares;
};

struct Board* createBoard(int width) {
  struct Board* board = (struct Board*) malloc(sizeof(struct Board));
  board->width = width;
  board->squares = (char*) malloc(width * width * sizeof(char));
  return board;
}

char get(struct Board *board, int row, int col) {
  return board->squares[row * board->width + col];
}

void setRow(struct Board *board, int row, char line[100]) {
  strncpy(&board->squares[row * board->width], line, board->width);
}

void printBoard(struct Board *board) {
  for (int r = 0; r < board->height; r++) {
    for (int c = 0; c < board->width; c++) {
      printf("%c", get(board, r, c));
    }
    printf("\n");
  }
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

  struct Board *board = NULL;
  
  char line[100];
  int lineCounter = 0;
  while(fgets(line, 100, fp)) {
    if (board == NULL) {
      board = createBoard(strlen(line)-1);
    }
    setRow(board, lineCounter++, line);
  }
  board->height = lineCounter;
  
  fclose(fp);

  printBoard(board);
}
