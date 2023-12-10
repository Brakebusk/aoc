#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tile {
  char symbol;
  char connections[4];
  int distance;
};

void move(int location[2], char direction) {
  switch (direction) {
    case 0:
      location[0]--;
      break;
    case 1:
      location[1]++;
      break;
    case 2:
      location[0]++;
      break;
    case 3:
      location[1]--;
      break;
    default:
      printf("Unknown direction '%d'\n", direction);
      exit(EXIT_FAILURE);
  }
}

int invertDirection(char direction) {
  return (direction + 2) % 4;
}

char connectionsToSymbol(char connections[4]) {
  if (connections[0] && connections[2]) return '|';
  if (connections[1] && connections[3]) return '-';
  if (connections[0] && connections[1]) return 'L';
  if (connections[0] && connections[3]) return 'J';
  if (connections[2] && connections[3]) return '7';
  if (connections[1] && connections[2]) return 'F';
  printf("Invalid connections\n");
  exit(EXIT_FAILURE);
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

  struct tile grid[150][150] = {0};
  int gridSize = 0;
  int start[2];

  char line[512];
  int lineIndex = 0;
  while(fgets(line, 512, fp)) {
    if (!gridSize) gridSize = strlen(line) - 1;

    for (int c = 0; c < gridSize; c++) {
      switch(line[c]) {
        case '|':
          grid[lineIndex][c] = (struct tile) {
            .connections = {1, 0, 1, 0},
            .distance = -1
          };
          break;
        case '-':
          grid[lineIndex][c] = (struct tile) {
            .connections = {0, 1, 0, 1},
            .distance = -1
          };
          break;
        case 'L':
          grid[lineIndex][c] = (struct tile) {
            .connections = {1, 1, 0, 0},
            .distance = -1
          };
          break;
        case 'J':
          grid[lineIndex][c] = (struct tile) {
            .connections = {1, 0, 0, 1},
            .distance = -1
          };
          break;
        case '7':
          grid[lineIndex][c] = (struct tile) {
            .connections = {0, 0, 1, 1},
            .distance = -1
          };
          break;
        case 'F':
          grid[lineIndex][c] = (struct tile) {
            .connections = {0, 1, 1, 0},
            .distance = -1
          };
          break;
        case '.':
          grid[lineIndex][c] = (struct tile) {
            .connections = {0, 0, 0, 0},
            .distance = -1
          };
          break;
        case 'S':
          grid[lineIndex][c] = (struct tile) {
            .connections = {0, 0, 0, 0},
            .distance = 0
          };
          start[0] = lineIndex;
          start[1] = c;
          break;
        default:
          printf("Unknown character '%c'\n", line[c]);
          exit(EXIT_FAILURE);
      }
      grid[lineIndex][c].symbol = line[c];
    }

    lineIndex++;
  }
  fclose(fp);

  if (start[0] > 0 && grid[start[0]-1][start[1]].connections[2]) grid[start[0]][start[1]].connections[0] = 1;
  if (start[0] < gridSize - 1 && grid[start[0]+1][start[1]].connections[0]) grid[start[0]][start[1]].connections[2] = 1;
  if (start[1] > 0 && grid[start[0]][start[1]-1].connections[1]) grid[start[0]][start[1]].connections[3] = 1;
  if (start[1] < gridSize - 1 && grid[start[0]][start[1]+1].connections[3]) grid[start[0]][start[1]].connections[1] = 1;
  grid[start[0]][start[1]].symbol = connectionsToSymbol(grid[start[0]][start[1]].connections);

  for (int d = 0; d < 4; d++) {
    if (grid[start[0]][start[1]].connections[d]) {
      int location[2] = {start[0], start[1]};
      
      int cameFrom = invertDirection(d);
      move(location, d);
      int currentDistance = 0;
      
      while (location[0] != start[0] || location[1] != start[1]) {
        currentDistance++;
        for (int nextD = 0; nextD < 4; nextD++) {
          if (nextD != cameFrom && grid[location[0]][location[1]].connections[nextD]) {
            if (grid[location[0]][location[1]].distance == -1 || grid[location[0]][location[1]].distance > currentDistance) {
              grid[location[0]][location[1]].distance = currentDistance;
            }
            move(location, nextD);
            cameFrom = invertDirection(nextD);
            break;
          }
        }

      }
    }
  }

  int part1 = 0;
  for (int l = 0; l < gridSize; l++) {
    for (int r = 0; r < gridSize; r++) {
      int distance = grid[l][r].distance;
      if (distance > part1) part1 = distance;
    }
  }

  int part2 = 0;
  for (int l = 0; l < gridSize; l++) {
    int inside = 0;
    int wallCount = 0;
    char prev = -1;
    for (int r = 0; r < gridSize; r++) {
      struct tile current = grid[l][r];
      
      if (current.distance >= 0 && current.symbol != '-') {
        wallCount++;
        if ((current.symbol == 'J' && prev == 'F') || (current.symbol == '7' && prev == 'L')) {
          wallCount--;
        }
        prev = current.symbol;
        inside = wallCount % 2 == 1;
      }
      
      if (inside && current.distance == -1) {
        part2++;
      }
    }
  }


  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}
