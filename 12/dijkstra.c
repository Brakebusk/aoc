#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Coordinate {
  int row;
  int col;
};

struct Square {
  char value;
  char visited;
  int distance;
};

struct Board {
  int width;
  int height;
  struct Square **squares;
  struct Coordinate *position;
  struct Coordinate *goal;
};

struct Board* createBoard(int width) {
  int squareCount = width * width;
  struct Board* board = (struct Board*) malloc(sizeof(struct Board));
  board->width = width;
  board->squares = malloc(squareCount * sizeof(struct Square*));
  for (int s = 0; s < squareCount; s++) {
    board->squares[s] = malloc(sizeof(struct Square));
    memset(board->squares[s], 0, sizeof(struct Square));
  }
  return board;
}

char getValue(struct Board *board, int row, int col) {
  return board->squares[row * board->width + col]->value;
}

char getVisited(struct Board *board, int row, int col) {
  return board->squares[row * board->width + col]->visited;
}

int getDistance(struct Board *board, int row, int col) {
  return board->squares[row * board->width + col]->distance;
}

void setValue(struct Board *board, int row, int col, char value) {
  board->squares[row * board->width + col]->value = value;
}

void setVisited(struct Board *board, int row, int col, char value) {
  board->squares[row * board->width + col]->visited = value;
}

void setDistance(struct Board *board, int row, int col, int distance) {
  board->squares[row * board->width + col]->distance = distance;
}

void defineStartEnd(struct Board *board) {
  struct Coordinate *start = malloc(sizeof(struct Coordinate));
  struct Coordinate *goal = malloc(sizeof(struct Coordinate));

  for (int r = 0; r < board->height; r++) {
    for (int c = 0; c < board->width; c++) {
      char value = getValue(board, r, c);
      if (value == 'S') {
        start->row = r;
        start->col = c;
        board->position = start;
        setValue(board, r, c, 'a');
      } else if (value == 'E') {
        goal->row = r;
        goal->col = c;
        board->goal = goal;
        setValue(board, r, c, 'z');
      }
    }
  }
}

void printBoard(struct Board *board) {
  printf("\n");
  for (int r = 0; r < board->height; r++) {
    for (int c = 0; c < board->width; c++) {
      printf("%c", getValue(board, r, c));
    }
    printf("\n");
  }
}

void printVisited(struct Board *board) {
  printf("\n");
  for (int r = 0; r < board->height; r++) {
    for (int c = 0; c < board->width; c++) {
      printf("%d", getVisited(board, r, c));
    }
    printf("\n");
  }
}

void printDistance(struct Board *board) {
  printf("\n");
  for (int r = 0; r < board->height; r++) {
    for (int c = 0; c < board->width; c++) {
      printf("%d", getDistance(board, r, c));
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
    for (int c = 0; c < board->width; c++) setValue(board, lineCounter, c, line[c]);
    lineCounter++;
  }
  board->height = lineCounter;
  defineStartEnd(board);
  
  fclose(fp);

  printBoard(board);
  printVisited(board);
  printDistance(board);
}
