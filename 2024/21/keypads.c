#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define LIMIT 504800

const char numeric[4][3] = {
  {'7', '8', '9'},
  {'4', '5', '6'},
  {'1', '2', '3'},
  { 0,  '0', 'A'},
};

const char directional[2][3] = {
  { 0,  '^', 'A'},
  {'<', 'v', '>'},
};

char paths[128][128]; 
int pathCount, shortestPath;

char segments[4][128][128];
char allShortestPaths[128][128];
int allShortestPathsCount;


int getLength(char path[128]) {
  for (int i = 0; i < 128; i++) {
    if (path[i] == 0) return i+1;
  }
  return INT_MAX;
}

void addSegment(int digit) {
  int shortest = INT_MAX;
  for (int p = 0; p < pathCount; p++) {
    int length = getLength(paths[p]);
    if (length < shortest) shortest = length;
  }
  int counter = 0;
  for (int p = 0; p < pathCount; p++) {
    int length = getLength(paths[p]);
    if (length == shortest) {
      memcpy(segments[digit][counter++], paths[p], 128);
    }
  }
}

void addPSegment(char pSegments[128][128][128], int digit) {
  int shortest = INT_MAX;
  for (int p = 0; p < pathCount; p++) {
    int length = getLength(paths[p]);
    if (length < shortest) shortest = length;
  }
  int counter = 0;
  for (int p = 0; p < pathCount; p++) {
    int length = getLength(paths[p]);
    if (length == shortest) {
      memcpy(pSegments[digit][counter++], paths[p], 128);
      if (counter >= 127) {
        printf("noooo\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

void addAllShortestPaths2(char pAllShortestPaths[LIMIT][128], int *pAllCount, char pSegments[128][128][128], int depth, char acc[128], int accLength) {
  if (!pSegments[depth][0][0]) {
    memcpy(pAllShortestPaths[(*pAllCount)++], acc, accLength);
    if (*pAllCount >= LIMIT-1) {
      printf("nonono\n");
      exit(EXIT_FAILURE);
    }
  } else {
    for (int p = 0; p < 128; p++) {
      if (strlen(pSegments[depth][p])) {
        strcpy(&acc[accLength], pSegments[depth][p]);
        addAllShortestPaths2(pAllShortestPaths, pAllCount, pSegments, depth+1, acc, strlen(acc));
      } else break;
    }
  }
}

void addAllShortestPaths() {
  allShortestPathsCount = 0;
  memset(allShortestPaths, 0, 128*128);
  for (int a = 0; a < 128; a++) {
    for (int b = 0; b < 128; b++) {
      for (int c = 0; c < 128; c++) {
        for (int d = 0; d < 128; d++) {
          if (segments[0][a][0] && segments[1][b][0] && segments[2][c][0] && segments[3][d][0]) {
            int length = 0;
            for (int i = 0; i < 128; i++) {
              if (segments[0][a][i]) {
                allShortestPaths[allShortestPathsCount][length++] = segments[0][a][i];
              } else {
                break;
              }
            }
            for (int i = 0; i < 128; i++) {
              if (segments[1][b][i]) {
                allShortestPaths[allShortestPathsCount][length++] = segments[1][b][i];
              } else {
                break;
              }
            }
            for (int i = 0; i < 128; i++) {
              if (segments[2][c][i]) {
                allShortestPaths[allShortestPathsCount][length++] = segments[2][c][i];
              } else {
                break;
              }
            }
            for (int i = 0; i < 128; i++) {
              if (segments[3][d][i]) {
                allShortestPaths[allShortestPathsCount][length++] = segments[3][d][i];
              } else {
                break;
              }
            }
            if (length >= 127) {
              printf("[error] too long shortest path!");
              exit(EXIT_FAILURE);
            }
            allShortestPathsCount++;
            if (allShortestPathsCount >= 127) {
              printf("[error] too many shortest paths!");
              exit(EXIT_FAILURE);
            }
          }
        } 
      }
    }
  }
}

void printAllShortest() {
  for (int i = 0; i < allShortestPathsCount; i++) {
    for (int c = 0; c < 128; c++) {
      if (allShortestPaths[i][c]) {
        printf("%c", allShortestPaths[i][c]);
      } else {
        break;
      }
    }
    printf("\n");
  }
}

void findAllNumericPaths(char visited[4][3], int goal, int r, int c, char currentPath[128], int pathLength) {
  if (shortestPath < pathLength) return;
  if (numeric[r][c] == goal) {
    currentPath[pathLength++] = 'A';
    if (pathLength < shortestPath) {
      pathCount = 0;
      shortestPath = pathLength;
    }
    if (pathCount >= 127) {
      printf("[error] too many paths!");
      exit(EXIT_FAILURE);
    }
    memset(paths[pathCount], 0, 128);
    memcpy(paths[pathCount++], currentPath, pathLength);
  } else {
    visited[r][c] = 1;

    if (r > 0 && !visited[r-1][c]) {
      currentPath[pathLength] = '^';
      findAllNumericPaths(visited, goal, r-1, c, currentPath, pathLength+1);
    }
    if (r < 3 && !visited[r+1][c]) {
      currentPath[pathLength] = 'v';
      findAllNumericPaths(visited, goal, r+1, c, currentPath, pathLength+1);
    }
    if (c > 0 && !visited[r][c-1]) {
      currentPath[pathLength] = '<';
      findAllNumericPaths(visited, goal, r, c-1, currentPath, pathLength+1);

    }
    if (c < 2 && !visited[r][c+1]) {
      currentPath[pathLength] = '>';
      findAllNumericPaths(visited, goal, r, c+1, currentPath, pathLength+1);
    }

    visited[r][c] = 0;
  }
}

void findAllDirectionalPaths(char visited[4][3], int goal, int r, int c, char currentPath[128], int pathLength) {
  if (shortestPath < pathLength) return;
  if (directional[r][c] == goal) {
    currentPath[pathLength++] = 'A';
    if (pathLength < shortestPath) {
      pathCount = 0;
      shortestPath = pathLength;
    }
    if (pathCount >= 127) {
      printf("[error] too many paths!");
      exit(EXIT_FAILURE);
    }
    memset(paths[pathCount], 0, 128);
    memcpy(paths[pathCount++], currentPath, pathLength);
  } else {
    visited[r][c] = 1;

    if (r > 0 && !visited[r-1][c]) {
      currentPath[pathLength] = '^';
      findAllDirectionalPaths(visited, goal, r-1, c, currentPath, pathLength+1);
    }
    if (r < 1 && !visited[r+1][c]) {
      currentPath[pathLength] = 'v';
      findAllDirectionalPaths(visited, goal, r+1, c, currentPath, pathLength+1);
    }
    if (c > 0 && !visited[r][c-1]) {
      currentPath[pathLength] = '<';
      findAllDirectionalPaths(visited, goal, r, c-1, currentPath, pathLength+1);

    }
    if (c < 2 && !visited[r][c+1]) {
      currentPath[pathLength] = '>';
      findAllDirectionalPaths(visited, goal, r, c+1, currentPath, pathLength+1);
    }

    visited[r][c] = 0;
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

  int part1 = 0;

  char line[8];
  while(fgets(line, 8, fp)) {
    memset(segments, 0, 4*128*128);

    char code[4];
    memcpy(code, line, 4);
    int row = 3, col = 2;
    for (int i = 0; i < 4; i++) {
      pathCount = 0;
      shortestPath = INT_MAX;
      char next = code[i];
      char path[128];
      char visited[4][3];
      memset(visited, 0, 4*3);
      visited[3][0] = 1;
      
      findAllNumericPaths(visited, next, row, col, path, 0);
      
      addSegment(i);
      
      for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 3; c++) {
          if (numeric[r][c] == next) {
            row = r;
            col = c;
            break;
          }
        }
      }
    }
    addAllShortestPaths();

    char (*trueShortest)[128] = malloc(128*LIMIT);
    int trueShortestLength = INT_MAX;
    int trueShortestCount = 0;

    for (int s = 0; s < allShortestPathsCount; s++) {
      char *sp = allShortestPaths[s];
      // example: <A^A^^>AvvvA

      char pSegments[128][128][128];
      memset(pSegments, 0, 128*128*128);

      row = 0, col = 2;
      for (int i = 0; i < getLength(sp); i++) {
        char dirVisited[2][3];
        memset(dirVisited, 0, 2*3);
        dirVisited[0][0] = 1;

        pathCount = 0;
        shortestPath = INT_MAX;
        char next = sp[i];
        char path[128];
        findAllDirectionalPaths(dirVisited, next, row, col, path, 0);

        addPSegment(pSegments, i);

        for (int r = 0; r < 2; r++) {
          for (int c = 0; c < 3; c++) {
            if (directional[r][c] == next) {
              row = r;
              col = c;
              break;
            }
          }
        }
      }

      char (*pAllShortestPaths)[128] = malloc(128*LIMIT);
      memset(pAllShortestPaths, 0, LIMIT*128);
      int pAllCount = 0;
      char acc[128] = {0};
      addAllShortestPaths2(pAllShortestPaths, &pAllCount, pSegments, 0, acc, 0);

      for (int j = 0; j < pAllCount; j++) {
        int l = strlen(pAllShortestPaths[j]);
        if (l < trueShortestLength) {
          trueShortestLength = l;
          trueShortestCount = 0;
        }
        if (l == trueShortestLength) {
          strcpy(trueShortest[trueShortestCount++], pAllShortestPaths[j]);
        }
      }
    }

    char (*truetrueShortest)[128] = malloc(128*LIMIT);
    int truetrueShortestLength = INT_MAX;
    int truetrueShortestCount = 0;

    for (int s = 0; s < trueShortestCount; s++) {
      char *sp = trueShortest[s];

      char pSegments[128][128][128];
      memset(pSegments, 0, 128*128*128);

      row = 0, col = 2;
      for (int i = 0; i < getLength(sp); i++) {
        char dirVisited[2][3];
        memset(dirVisited, 0, 2*3);
        dirVisited[0][0] = 1;

        pathCount = 0;
        shortestPath = INT_MAX;
        char next = sp[i];
        char path[128];
        findAllDirectionalPaths(dirVisited, next, row, col, path, 0);

        addPSegment(pSegments, i);

        for (int r = 0; r < 2; r++) {
          for (int c = 0; c < 3; c++) {
            if (directional[r][c] == next) {
              row = r;
              col = c;
              break;
            }
          }
        }
      }

      char (*pAllShortestPaths)[128] = malloc(128*LIMIT);
      memset(pAllShortestPaths, 0, LIMIT*128);
      int pAllCount = 0;
      char acc[128] = {0};
      addAllShortestPaths2(pAllShortestPaths, &pAllCount, pSegments, 0, acc, 0);

      for (int j = 0; j < pAllCount; j++) {
        int l = strlen(pAllShortestPaths[j]);
        if (l < truetrueShortestLength) {
          truetrueShortestLength = l;
          truetrueShortestCount = 0;
        }
        if (l == truetrueShortestLength) {
          strcpy(truetrueShortest[truetrueShortestCount++], pAllShortestPaths[j]);
        }
      }
    }

    char numeric[4];
    memset(numeric, 0, 4);
    memcpy(numeric, line, 3);
    part1 += truetrueShortestLength * atoi(numeric);
  }
  fclose(fp);

  printf("Part 1: %d\n", part1);
}
