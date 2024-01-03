#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CARD_COUNT 10

int mod(int a, int b) {
  return (a % b + b) % b;
}

void deal(int *from, int *to) {
  for (int i = 0; i < CARD_COUNT; i++) {
    to[CARD_COUNT-1-i] = from[i];
    from[i] = -1;
  }
}

void dealIncrement(int *from, int *to, int increment) {
  for (int i = 0; i < CARD_COUNT; i++) {
    to[mod(i*increment, CARD_COUNT)] = from[i];
    from[i] = -1;
  }
}

void cut(int *from, int *to, int n) {
  for (int i = CARD_COUNT-1; i >= 0; i--) {
    to[i] = from[mod(i + n, CARD_COUNT)];
    from[mod(i + n, CARD_COUNT)] = -1;
  }
}

void printDeck (int *deck) {
  for (int i = 0; i < CARD_COUNT; i++) {
    printf("%d ", deck[i]);
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

  int *deck = malloc(sizeof(int) * CARD_COUNT);
  int *deck2 = malloc(sizeof(int) * CARD_COUNT);
  for (int i = 0; i < CARD_COUNT; i++) {
    deck[i] = i;
  }
  

  char line[512];
  while(fgets(line, 512, fp)) {

  }
  fclose(fp);

  free(deck);
  free(deck2);
}
