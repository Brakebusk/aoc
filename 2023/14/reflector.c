#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printGrid(char grid[128][128], int gridSize, char *caption) {
  printf("%s:\n", caption);
  for (int row = 0; row < gridSize; row++) {
    for (int col = 0; col < gridSize; col++) {
      printf("%c", grid[row][col]);
    }
    printf("\n");
  }
  printf("\n");
}

int isRoundRock(char symbol) {
  return symbol == 'O';
}

int isCubedRock(char symbol) {
  return symbol == '#';
}

int isEmpty(char symbol) {
  return symbol == '.';
}

void tiltNorth(char grid[128][128], int gridSize) {
  for (int row = 0; row < gridSize; row++) {
    for (int col = 0; col < gridSize; col++) {
      if (isRoundRock(grid[row][col])) {
        int localRow = row;
        while (localRow > 0 && isEmpty(grid[localRow-1][col])) {
          grid[localRow][col] = '.';
          grid[--localRow][col] = 'O';
        }
      }
    }
  } 
}

void tiltSouth(char grid[128][128], int gridSize) {
  for (int row = gridSize-1; row >= 0; row--) {
    for (int col = 0; col < gridSize; col++) {
      if (isRoundRock(grid[row][col])) {
        int localRow = row;
        while (localRow < gridSize-1 && isEmpty(grid[localRow+1][col])) {
          grid[localRow][col] = '.';
          grid[++localRow][col] = 'O';
        }
      }
    }
  } 
}

void tiltEast(char grid[128][128], int gridSize) {
  for (int col = gridSize-1; col >= 0; col--) {
    for (int row = 0; row < gridSize; row++) {
      if (isRoundRock(grid[row][col])) {
        int localCol = col;
        while (localCol < gridSize-1 && isEmpty(grid[row][localCol+1])) {
          grid[row][localCol] = '.';
          grid[row][++localCol] = 'O';
        }
      }
    }
  } 
}

void tiltWest(char grid[128][128], int gridSize) {
  for (int col = 0; col < gridSize; col++) {
    for (int row = 0; row < gridSize; row++) {
      if (isRoundRock(grid[row][col])) {
        int localCol = col;
        while (localCol > 0 && isEmpty(grid[row][localCol-1])) {
          grid[row][localCol] = '.';
          grid[row][--localCol] = 'O';
        }
      }
    }
  } 
}

void cycle(char grid[128][128], int gridSize, int skipNorth) {
  if (!skipNorth) tiltNorth(grid, gridSize);
  tiltWest(grid, gridSize);
  tiltSouth(grid, gridSize);
  tiltEast(grid, gridSize);
}

int getLoad(char grid[128][128], int gridSize) {
  int load = 0;
  for (int row = 0; row < gridSize; row++) {
    for (int col = 0; col < gridSize; col++) {
      if (isRoundRock(grid[row][col])) load += gridSize-row;
    }
  }
  return load;
};

int inHistory(int history[512], int historyLength, int value) {
  for (int i = 0; i < historyLength; i++) {
    if (history[i] == value) return i;
  }
  return -1;
}

int getNextInHistory(int history[512], int historyLength, int after, int value) {
  for (int i = after+1; i < historyLength; i++) {
    if (history[i] == value) return i;
  }
  return -1;
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

  char grid[128][128];
  int gridSize = 0;

  int row = 0;
  char line[128];
  while(fgets(line, 128, fp)) {
    if (!gridSize) gridSize = strlen(line) - 1;
    memcpy(grid[row++], line, gridSize);
  }
  fclose(fp);

  tiltNorth(grid, gridSize);

  int part1 = getLoad(grid, gridSize);

  cycle(grid, gridSize, 1);

  int history[512];
  int historyLength = 0;

  int foundPatternAt = 0;
  int pattern[128];
  int patternLength = 0;

  // Generate a long enough history for the pattern to emerge
  int cycleCount;
  int currentLoad = getLoad(grid, gridSize);
  history[historyLength++] = currentLoad;
  for (cycleCount = 1; cycleCount < 512; cycleCount++) {
    cycle(grid, gridSize, 0);
    currentLoad = getLoad(grid, gridSize);
    history[historyLength++] = currentLoad;
  }
  
  // Find the pattern
  for (int h = 0; h < historyLength - 7; h++) {
    int v1 = history[h];
    int nextv1 = getNextInHistory(history, historyLength, h+3, v1);
    int v2 = history[h+1];
    int nextv2 = getNextInHistory(history, historyLength, nextv1, v2);
    int v3 = history[h+2];
    int nextv3 = getNextInHistory(history, historyLength, nextv2, v3);

    if (nextv2 - nextv1 == 1 && nextv3 - nextv2 == 1) {
      for (int i = h; i < nextv1; i++) {
        pattern[patternLength++] = history[i];
      }
      foundPatternAt = h;
      break;
    }
  }

  int part2 = pattern[(1000000000 - foundPatternAt - 1) % patternLength];

  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}
