#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct hand {
  int cards[5];
  int bid;
  int type;
};

int compareHands(const void *a, const void* b) {
  struct hand hand1 = *(struct hand*)a;
  struct hand hand2 = *(struct hand*)b;

  if (hand1.type > hand2.type) return 1;
  if (hand1.type < hand2.type) return -1;

  for (int c = 0; c < 5; c++) {
    int h1c = hand1.cards[c];
    int h2c = hand2.cards[c];
    if (h1c > h2c) return 1;
    if (h1c < h2c) return -1;
  }

  return 0;
}

int cardSymbolToValue(char symbol) {
  switch (symbol) {
    case 'A':
      return 14;
      break;
    case 'K':
      return 13;
      break;
    case 'Q':
      return 12;
      break;
    case 'J':
      return 11;
      break;
    case 'T':
      return 10;
      break;
    default:
      return symbol - '0';
  }
}

/*
  6 = five of a kind
  5 = four of a kind
  4 = full house
  3 = three of a kind
  2 = two pair
  1 = one pair
  0 = high card
*/
int getHandType(int cards[5]) {
  int counts[15] = {0};
  for (int c = 0; c < 5; c++) counts[cards[c]]++;
  int jokerCount = counts[1];

  for (int v = 2; v < 15; v++) {
    if (counts[v]+jokerCount == 5) return 6;
  }

  for (int v = 2; v < 15; v++) {
    if (counts[v]+jokerCount == 4) return 5;
  }

  int hasThreeAlike = 0;
  int pairCount = 0;

  for (int v = 2; v < 15; v++) {
    if (counts[v]+jokerCount == 3) {
      hasThreeAlike = 1;
      if (jokerCount) {
        counts[v] = 3;
        jokerCount = 0;
      }
      break;
    }
  }

  for (int v = 2; v < 15; v++) {
    if (counts[v]+jokerCount == 2) {
      pairCount++;
      if (jokerCount) jokerCount = 0;
    }
  }

  if (hasThreeAlike) return pairCount ? 4 : 3;
  return pairCount;
}

void convertHandsToPart2(struct hand hands[], int handCount) {
  for (int h = 0; h < handCount; h++) {
    // Convert jacks to jokers (value = 1)
    for (int c = 0; c < 5; c++) {
      if (hands[h].cards[c] == 11) hands[h].cards[c] = 1;
    }

    hands[h].type = getHandType(hands[h].cards);
  }
}

int calculateWinnings(struct hand hands[], int handCount) {
  int winnings = 0;
  for (int h = 0; h < handCount; h++) {
    int rank = h + 1;
    int bid = hands[h].bid;
    winnings += rank * bid;
  }
  return winnings;
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

  struct hand hands[1024];
  int handCount = 0;

  char line[16];
  while(fgets(line, 16, fp)) {
    char *token = strtok(line, " ");
    for (int i = 0; i < 5; i++) {
      hands[handCount].cards[i] = cardSymbolToValue(token[i]);
    }
    token = strtok(NULL, "\n");
    hands[handCount].bid = atoi(token);
    hands[handCount].type = getHandType(hands[handCount].cards);
    handCount++;
  }
  fclose(fp);

  qsort(hands, handCount, sizeof(struct hand), compareHands);

  int part1 = calculateWinnings(hands, handCount);

  convertHandsToPart2(hands, handCount);
  qsort(hands, handCount, sizeof(struct hand), compareHands);

  int part2 = calculateWinnings(hands, handCount);

  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}
