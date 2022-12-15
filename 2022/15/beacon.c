#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int min(int a, int b, int c) {
  int tmp = a > b ? b : a;
  return tmp > c ? c : tmp;
}

int max(int a, int b, int c) {
  int tmp = a > b ? a : b;
  return tmp > c ? tmp : c;
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

struct Slice {
  int width;
  int height;
  int offsetX;
  int offsetY;
  char* grid;
};

char get(struct Slice* slice, int x, int y) {
  return slice->grid[((y + slice->offsetY) * slice->width) + x + slice->offsetX];
}

void set(struct Slice* slice, int x, int y, char value) {
  slice->grid[((y + slice->offsetY) * slice->width) + x + slice->offsetX] = value;
}

struct Slice* createSlice(int width, int height, int offsetX, int offsetY) {
  struct Slice* slice = (struct Slice*) malloc(sizeof(struct Slice));
  slice->width = width;
  slice->height = height;
  slice->offsetX = offsetX;
  slice->offsetY = offsetY;
  slice->grid = malloc(width * height * sizeof(char));
  memset(slice->grid, 0, width * height * sizeof(char));
  return slice;
}

void populate(struct Slice* slice, struct Entry* entries[50], int entryCount) {
  for (int i = 0; i < entryCount; i++) {
    struct Entry* entry = entries[i];
    set(slice, entry->sx, entry->sy, Sensor);
    set(slice, entry->bx, entry->by, Beacon);

    printf("Set at S(%d, %d) B(%d, %d)\n", entry->sx, entry->sy, entry->bx, entry->by);
  }
}

void debug(struct Slice* slice) {
  for (int y = -(slice->offsetY); y < slice->height - slice->offsetY; y++) {
    printf("%2d ", y);
    for (int x = -(slice->offsetX); x <= slice->width - slice->offsetX; x++) {
      if (x == -(slice->offsetX)) printf("%2d ", x);
      switch(get(slice, x, y)) {
        case Air:
          printf(".");
          break;
        case Impossible:
          printf("#");
          break;
        case Sensor:
          printf("S");
          break;
        case Beacon:
          printf("B");
          break;
        default:
          printf("[SANITY] Invalid material %d\n", get(slice, x, y));
          exit(EXIT_FAILURE);
          break;
      }
      if (x == slice->width - slice->offsetX) printf(" %d", x);
    }
    printf("\n");
  }
  printf("\n");
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

  struct Entry* entries[50];
  int entryCount = 0;

  int minX = 0, maxX = 0, minY = 0, maxY = 0;
  
  char line[100];
  while(fgets(line, 100, fp)) {
    int sx, sy, bx, by;
    sscanf(line, "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", &sx, &sy, &bx, &by);
    struct Entry* sensor = createEntry(sx, sy, bx, by);
    entries[entryCount++] = sensor;

    minX = min(sx, bx, minX);
    maxX = max(sx, bx, maxX);
    minY = min(sy, by, minY);
    maxY = max(sy, by, maxY);
  }

  int width = abs(minX) + abs(maxX);
  int height = abs(minY) + abs(maxY) + 1;

  printf("Width: %d (offset %d), Height: %d (offset %d)\n", width, abs(min(0, 0, minX)), height, abs(min(0, 0, minY)));

  struct Slice* slice = createSlice(width, height, abs(min(0, 0, minX)), abs(min(0, 0, minY)));
  populate(slice, entries, entryCount);

  debug(slice);
  
  fclose(fp);
}