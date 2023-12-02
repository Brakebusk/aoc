#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int min(int a, int b) {
  return a > b ? b : a;
}

int max(int a, int b) {
  return a > b ? a : b;
}

enum Element {
  Air = 0,
  Impossible = 1,
  Sensor = 2,
  Beacon = 3,
};

struct Entry {
  int sx;
  int sy;
  int bx;
  int by;
};

struct Entry* createEntry(int sx, int sy, int bx, int by) {
  struct Entry* entry = (struct Entry*) malloc(sizeof(struct Entry));
  entry->sx = sx;
  entry->sy = sy;
  entry->bx = bx;
  entry->by = by;
  return entry;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("[ERROR] Missing parameter <filename> <line>\n");
    exit(EXIT_FAILURE);
  }

  char *filename = argv[1];
  int scanLine = atoi(argv[2]);

  FILE *fp = NULL;
  if ((fp = fopen(filename, "r")) == NULL) {
      printf("[ERROR] Failed to open file %s\n", filename);
      exit(EXIT_FAILURE);
  }

  struct Entry* entries[50];
  int entryCount = 0;
  
  char line[100];
  while(fgets(line, 100, fp)) {
    int sx, sy, bx, by;
    sscanf(line, "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", &sx, &sy, &bx, &by);
    struct Entry* sensor = createEntry(sx, sy, bx, by);
    entries[entryCount++] = sensor;
  }
  fclose(fp);
  
  char* occupied = malloc(1000000 * sizeof(char));
  memset(occupied, 0, 1000000 * sizeof(char));

  for (int i = 0; i < entryCount; i++) {
    struct Entry* entry = entries[i];
    int xOffset = abs(entry->sx - entry->bx);
    int yOffset = abs(entry->sy - entry->by);
    int distance = yOffset + xOffset;


    int minY = entry->sx - distance;
    int maxY = entry->sx + distance;

    if (minY <= scanLine && maxY >= scanLine) {
      int scanLineOffset = distance - abs(entry->sy - scanLine);
      
      for (int c = entry->sx - scanLineOffset; c <= entry->sx + scanLineOffset; c++) {
        occupied[c + 500000] = 1; 
      }

    }
  }

  int occupiedCount = 0;
  for (int i = 0; i < 1000000; i++) {
    if (occupied[i] > 0) occupiedCount++;
  }

  printf("Part 1: %d\n", occupiedCount);
  
}