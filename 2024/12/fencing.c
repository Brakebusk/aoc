#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REGIONS 1024

int markRegion(int matrix[150][150], int size, int r, int c, int id) {
  int count = 1;
  int plant = matrix[r][c];
  matrix[r][c] = id;

  if (r > 0 && matrix[r-1][c] == plant) count += markRegion(matrix, size, r-1, c, id);
  if (r < size - 1 && matrix[r+1][c] == plant) count += markRegion(matrix, size, r+1, c, id);
  if (c > 0 && matrix[r][c-1] == plant) count += markRegion(matrix, size, r, c-1, id);
  if (c < size - 1 && matrix[r][c+1] == plant) count += markRegion(matrix, size, r, c+1, id);

  return count;
}

int countPerimeter(int matrix[150][150], int size, int r, int c, int id) {
  matrix[r][c] = -id;
  int count = 0;
  if (r > 0) {
    if (matrix[r-1][c] == id) {
      count += countPerimeter(matrix, size, r-1, c, id);
    } else if (matrix[r-1][c] != -id) count++;
  } else count++;
  if (r < size - 1) {
    if (matrix[r+1][c] == id) {
      count += countPerimeter(matrix, size, r+1, c, id);
    } else if (matrix[r+1][c] != -id) count++;
  } else count++;
  if (c > 0) {
    if (matrix[r][c-1] == id) {
      count += countPerimeter(matrix, size, r, c-1, id);
    } else if (matrix[r][c-1] != -id) count++;
  } else count++;
  if (c < size - 1) {
    if (matrix[r][c+1] == id) {
      count += countPerimeter(matrix, size, r, c+1, id);
    } else if (matrix[r][c+1] != -id) count++;
  } else count++;
  return count;
}

int countCorners(int matrix[150][150], int size, int r, int c) {
  char neighbours[3][3] = {
    {0, 0, 0},
    {0, 1, 0},
    {0, 0, 0},
  };
  int id = matrix[r][c];
  if (r > 0) {
    if (c > 0 && matrix[r-1][c-1] == id) neighbours[0][0] = 1;
    if (matrix[r-1][c] == id) neighbours[0][1] = 1;
    if (c < size-1 && matrix[r-1][c+1] == id) neighbours[0][2] = 1;
  }
  if (c > 0 && matrix[r][c-1] == id) neighbours[1][0] = 1;
  if (c < size-1 && matrix[r][c+1] == id) neighbours[1][2] = 1;
  if (r < size-1) {
    if (c > 0 && matrix[r+1][c-1] == id) neighbours[2][0] = 1;
    if (matrix[r+1][c] == id) neighbours[2][1] = 1;
    if (c < size-1 && matrix[r+1][c+1] == id) neighbours[2][2] = 1;
  }

  int corners = 0;
  if ((neighbours[0][1] == 1 && neighbours[1][0] == 1 && neighbours[0][0] == 0) || (neighbours[0][1] == 0 && neighbours[1][0] == 0)) corners++;
  if ((neighbours[0][1] == 1 && neighbours[1][2] == 1 && neighbours[0][2] == 0) || (neighbours[0][1] == 0 && neighbours[1][2] == 0)) corners++;
  if ((neighbours[1][0] == 1 && neighbours[2][1] == 1 && neighbours[2][0] == 0) || (neighbours[1][0] == 0 && neighbours[2][1] == 0)) corners++;
  if ((neighbours[1][2] == 1 && neighbours[2][1] == 1 && neighbours[2][2] == 0) || (neighbours[1][2] == 0 && neighbours[2][1] == 0)) corners++;
  return corners;
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

  int matrix[150][150];
  int size = 0;

  char line[150];
  int lc = 0;
  while(fgets(line, 150, fp)) {
    if (size == 0) size = strlen(line) - 1;
    for (int c = 0; c < size; c++) matrix[lc][c] = line[c] + MAX_REGIONS;
    lc++;
  }
  fclose(fp);

  int regionSizes[MAX_REGIONS] = {0};
  int regionPerimeters[MAX_REGIONS] = {0};
  int regionSides[MAX_REGIONS] = {0};
  int regionCount = 0;

  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      if (matrix[r][c] >= 'A' + MAX_REGIONS) {
        regionSizes[regionCount] = markRegion(matrix, size, r, c, regionCount+1);
        regionCount++;
      }
    }
  }

  int perimeterId = 1;
  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      if (matrix[r][c] == perimeterId) {
        regionPerimeters[perimeterId-1] = countPerimeter(matrix, size, r, c, perimeterId);
        perimeterId++;
      }
    }
  }

  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      regionSides[-matrix[r][c]-1] += countCorners(matrix, size, r, c);
    }
  }
  
  int part1 = 0;
  int part2 = 0;
  for (int i = 0; i < regionCount; i++) {
    part1 += regionSizes[i] * regionPerimeters[i];
    part2 += regionSizes[i] * regionSides[i];
  }
  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}
