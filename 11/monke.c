#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROUNDS 10000
#define WORRYREDUCTION 0

struct Monke {
  long inspections;
  long items[50];
  int itemCount;

  char operation;
  int magnitude;
  int test;
  int testTrue;
  int testFalse;
};

struct Monke* spawnMonke() {
  struct Monke* m = (struct Monke*) malloc(sizeof(struct Monke));
  m->inspections = 0;
  m->itemCount = 0;
  m->magnitude = 0;
  return m;
}

void push(struct Monke* monke, long value) {
  monke->items[monke->itemCount++] = value;
}

long pop(struct Monke* monke) {
  long value = monke->items[0];
  for (int i = 0; i < monke->itemCount - 1; i++) {
    monke->items[i] = monke->items[i+1];
  }  
  monke->itemCount--;
  return value;
}

long inspect(struct Monke* monke, int lcm) {
  monke->inspections++;
  long item = pop(monke);

  switch (monke->operation) {
    case '+':
      item += monke->magnitude;
      break;
    case '*':
      if (!monke->magnitude) {
        item *= item;
      } else {
        item *= monke->magnitude;
      }
      break;
    default:
      printf("Invalid operation '%c'\n", monke->operation);
      exit(EXIT_FAILURE);
  }

  return WORRYREDUCTION ? item / WORRYREDUCTION : item % lcm;
}

long activityProduct(struct Monke* monkes[10], int monkeCount) {
  long max1 = 0;
  long max2 = 0;
  for (int m = 0; m <= monkeCount; m++) {
    long inspections = monkes[m]->inspections;
    if (inspections > max1) {
      max2 = max1;
      max1 = inspections;
    } else if (inspections > max2) {
      max2 = inspections;
    }
  }

  return max1 * max2;
}

int findLCM(int a, int b) {
  int max = (a > b) ? a : b;

  while (1) {
    if ((max % a == 0) && (max % b == 0)) return max;
    max++;
  }
}

void debugMonkeState(struct Monke* monke) {
  printf("Inspections: %ld\nItems: ", monke->inspections);
  for (int i = 0; i < monke->itemCount; i++) printf(" '%ld'", monke->items[i]);
  printf("\nOperation: '%c' with '%d'\nTest: divisible by '%d'\nIf true -> '%d'\nIf False -> '%d'\n\n", monke->operation, monke->magnitude, monke->test, monke->testTrue, monke->testFalse);
}

void debugRound(int round, struct Monke* monkes[10], int monkeCount) {
  printf("After round %d:\n", round);
  for (int i = 0; i <= monkeCount; i++) {
    printf("Monke %d:", i);
    for (int j = 0; j < monkes[i]->itemCount; j++) printf(" '%ld'", monkes[i]->items[j]);
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

  struct Monke* monkes[10];
  int monkeCount = 0;
  int definitionStep = 0;
  
  char line[100];
  while(fgets(line, 100, fp)) {
    switch (definitionStep) {
      case 0:
        monkes[monkeCount] = spawnMonke();
        break;
      case 1: {
        char *token;
        token = strtok(line, ":");
        token = strtok(NULL, ",");
        while (token != NULL) {
          push(monkes[monkeCount], atoi(token));
          token = strtok(NULL, ",");
        }
        break;
      }
      case 2: {
        char operation;
        char magnitude[5] = {0};
        sscanf(line, "  Operation: new = old %c %s", &operation, magnitude);
        monkes[monkeCount]->operation = operation;
        if (strcmp("old", magnitude) != 0) monkes[monkeCount]->magnitude = atoi(magnitude);
        break;
      }
      case 3: {
        int test;
        sscanf(line, "  Test: divisible by %d", &test);
        monkes[monkeCount]->test = test;
        break;
      }
      case 4: {
        int testTrue;
        sscanf(line, "    If true: throw to monkey %d", &testTrue);
        monkes[monkeCount]->testTrue = testTrue;
        break;
      }
      case 5: {
        int testFalse;
        sscanf(line, "    If false: throw to monkey %d", &testFalse);
        monkes[monkeCount]->testFalse = testFalse;
        break;
      }
      case 6:
        monkeCount++;
        definitionStep = -1;
        break;
    }
    definitionStep++;
  }
  
  fclose(fp);

  int lcm = 1;
  for (int m = 0; m <= monkeCount; m++) {
    lcm = findLCM(lcm, monkes[m]->test);
  }

  for (int r = 1; r <= ROUNDS; r++) {
    for (int m = 0; m <= monkeCount; m++) {
      struct Monke* monke = monkes[m];
      while (monke->itemCount > 0) {
        long item = inspect(monke, lcm);
        push(monkes[item % monke->test == 0 ? monke->testTrue : monke->testFalse], item);
      }
    }
  }

  printf("Part X: %ld\n", activityProduct(monkes, monkeCount));
}