#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct pair {
  char lsb, delta;
};

struct pair monkeys[1700][2001];
int monkeyCount = 0;

char lsd(long long value) {
  return value % 10;
}

long long getBestSequence() {
  long long best = 0;
  for (int a = -9; a < 10; a++) {
    for (int b = -9; b < 10; b++) {
      for (int c = -9; c < 10; c++) {
        for (int d = -9; d < 10; d++) {
          long long acc = 0;
          for (int m = 0; m < monkeyCount; m++) {
            for (int i = 1; i < 2001-4; i++) {
              if (monkeys[m][i].delta == a && monkeys[m][i+1].delta == b && monkeys[m][i+2].delta == c && monkeys[m][i+3].delta == d) {
                acc += monkeys[m][i+3].lsb;
                break;
              }
            }
          }
          if (acc > best) best = acc;
        }
      }
    }
  }
  return best;
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

  long long part1 = 0;

  char line[16];
  while(fgets(line, 16, fp)) {
    long long secret = atoll(line);

    monkeys[monkeyCount][0].lsb = lsd(secret);

    for (int i = 0; i < 2000; i++) {
      secret ^= secret * 64;
      secret %= 16777216;
      secret ^= secret / 32;
      secret %= 16777216;
      secret ^= secret * 2048,
      secret %= 16777216;

      monkeys[monkeyCount][i+1].lsb = lsd(secret);
      monkeys[monkeyCount][i+1].delta = lsd(secret) - monkeys[monkeyCount][i].lsb;
    }
    part1 += secret;
    monkeyCount++;
  }
  fclose(fp);

  printf("Part 1: %lld\n", part1);
  printf("Part 2: %lld\n", getBestSequence());
}
