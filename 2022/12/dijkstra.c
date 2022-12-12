#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

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

struct Neighbors {
  struct Coordinate neighbors[4];
  int count;
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

void setVisited(struct Board *board, int row, int col ) {
  board->squares[row * board->width + col]->visited = 1;
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
      setDistance(board, r, c, INT_MAX);
      if (value == 'S') {
        start->row = r;
        start->col = c;
        board->goal = start;
        setValue(board, r, c, 'a');
      } else if (value == 'E') {
        goal->row = r;
        goal->col = c;
        board->position = goal;
        setValue(board, r, c, 'z');
        setDistance(board, r, c, 0);
      }
    }
  }
}

struct Neighbors* findAccessibleUnvisitedNeighbors(struct Board *board) {
  struct Neighbors *neighbors = malloc(sizeof(struct Neighbors));
  neighbors->count = 0;

  int currentRow = board->position->row;
  int currentCol = board->position->col;
  char currentValue = getValue(board, currentRow, currentCol);

  int deltaX[4] = {0, 1, 0, -1};
  int deltaY[4] = {-1, 0, 1, 0};

  for (int i = 0; i < 4; i++) {
    int nR = currentRow + deltaY[i];
    int nC = currentCol + deltaX[i];

    if (nR >= 0 && 
        nR < board->height && 
        nC >= 0 &&
        nC < board->width &&
        getVisited(board, nR, nC) == 0 &&
        currentValue - getValue(board, nR, nC) <= 1) {
          neighbors->neighbors[neighbors->count].row = nR;
          neighbors->neighbors[neighbors->count].col = nC;
          neighbors->count++;
    }
  }

  return neighbors;
}

void findNextUnvisitedSquare(struct Board *board) {
  int smallestDistance = INT_MAX;
  struct Coordinate pos;
  for (int r = 0; r < board->height; r++) {
    for (int c = 0; c < board->width; c++) {
      int distance;
      if (getVisited(board, r, c) == 0 && (distance = getDistance(board, r, c)) < smallestDistance) {
        smallestDistance = distance;
        pos.row = r;
        pos.col = c;
      }
    }
  }
  board->position->row = pos.row;
  board->position->col = pos.col;
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
      printf("%d ", getDistance(board, r, c));
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

  while (board->position->row != board->goal->row || board->position->col != board->goal->col) {
    struct Coordinate* currentPosition = board->position;

    int currentDistance = getDistance(board, currentPosition->row, currentPosition->col);
    struct Neighbors* neighbors = findAccessibleUnvisitedNeighbors(board);

    for (int n = 0; n < neighbors->count; n++) {
      struct Coordinate selected = neighbors->neighbors[n];
      int selectedDistance = getDistance(board, selected.row, selected.col);
      if (selectedDistance > currentDistance + 1) {
        setDistance(board, selected.row, selected.col, currentDistance + 1);
      }
    }

    free(neighbors);
    setVisited(board, currentPosition->row, currentPosition->col);
    findNextUnvisitedSquare(board);
  }
  
  printf("Part 1: %d\n", getDistance(board, board->position->row, board->position->col));

  int smallesAZDistance = INT_MAX;
  for (int r = 0; r < board->height; r++) {
    for (int c = 0; c < board->width; c++) {
      int distance;
      if (getValue(board, r, c) == 'a' && (distance = getDistance(board, r, c)) < smallesAZDistance) {
        smallesAZDistance = distance;
      }
    }
  }
  printf("Part 2: %d\n", smallesAZDistance);
}
