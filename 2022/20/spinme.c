#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int mod(int a, int b) {
  return (a % b + b) % b;
}

struct Value {
  int value;
  int moved;
};

struct Circle {
  struct Value values[10000];
  int length;
};

struct Circle* createCircle() {
  struct Circle* circle = (struct Circle*) malloc(sizeof(struct Circle));
  circle->length = 0;
  return circle;
}

void add(struct Circle* circle, int value) {
  struct Value* v = &circle->values[circle->length++];
  v->value = value;
  v->moved = 0;
}

int get(struct Circle* circle, int index) {
  return circle->values[mod(index, circle->length)].value;
}

int getMoved(struct Circle* circle, int index) {
  return circle->values[mod(index, circle->length)].moved;
}

void setMoved(struct Circle* circle, int index) {
  circle->values[mod(index, circle->length)].moved = 1;
}

void swap(struct Circle* circle, int index, int direction) {
  int tmpValue = get(circle, index);
  int tmpMoved = getMoved(circle, index);
  circle->values[mod(index, circle->length)].value = circle->values[mod(index + direction, circle->length)].value;
  circle->values[mod(index, circle->length)].moved = circle->values[mod(index + direction, circle->length)].moved;
  circle->values[mod(index + direction, circle->length)].value = tmpValue;
  circle->values[mod(index + direction, circle->length)].moved = tmpMoved;
}

int run(struct Circle* circle) {
  for (int i = 0; i < circle->length; i++) printf("%d, ", get(circle, i));
  printf("\n");

  for (int i = 0; i < circle->length; i++) {
    if (getMoved(circle, i) == 0) {
      setMoved(circle, i);
      int value = get(circle, i);
      int newPos = mod(i + value, circle->length);
      int delta = newPos - i;
      int direction = delta > 0 ? 1 : -1;

      printf("From %d to %d = delta %d direction %d\n", i, newPos, delta, direction);

      for (int m = 0; m < abs(delta - (direction > 0 ? 0 : 1)); m++) {
        swap(circle, i + (m * direction), direction);
      }
      return 1;
    }
  }

  return 0;
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

  struct Circle* circle = createCircle();
  
  char line[10];
  while(fgets(line, 10, fp)) {
    int value;
    sscanf(line, "%d", &value);
    add(circle, value);
  }
  fclose(fp);

  while (run(circle) == 1) {
    continue;
  }

  printf("Part 1: %d\n", get(circle, 1000) + get(circle, 2000) + get(circle, 3000));
}