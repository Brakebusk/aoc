#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define THREAD_COUNT 32

int basePattern[4] = {0, 1, 0, -1};
int getPatternValue(int digit, int index) {
  int repeats = digit + 1;

  return basePattern[((index + 1) / repeats) % 4];
}

void printDigits(int *digits, int digitCount, int offset) {
  for (int i = offset; i < digitCount+offset; i++) printf("%d", digits[i]);
  printf("\n");
}

struct arg_struct {
  int start, end, trueDigitCount;
  int *digits;
  int *output;
};

void *thread(void *arguments) {
  struct arg_struct *args = (struct arg_struct *)arguments;

  for (int d = args->start; d <= args->end; d++) {
    int v = 0;
    for (int j = 0; j < args->trueDigitCount; j++) {
      v += getPatternValue(d, j) * args->digits[j];
    }
    args->output[d] = abs(v) % 10;
  }

  return NULL;
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

  char line[800];
  fgets(line, 800, fp);
  fclose(fp);

  int digitCount = strlen(line);
  int *digits = malloc(digitCount * sizeof(int) * 10001);
  for (int d = 0; d < digitCount; d++) digits[d] = line[d] - '0';
  for (int r = 0; r < 10000; r++) {
    memcpy(&digits[(r+1) * digitCount], digits, sizeof(int) * digitCount);
  }

  int *output = malloc(digitCount * sizeof(int) * 10001);

  char offsetDigits[8];
  strncpy(offsetDigits, line, 7);
  int offset = atoi(offsetDigits);

  int trueDigitCount = digitCount*10000;

  pthread_t threads[THREAD_COUNT];
  struct arg_struct args[THREAD_COUNT];

  for (int i = 0; i < 100; i++) {
    printf("Round #%d\n", i+1);

    int threadCount;

    int segmentSize = trueDigitCount / THREAD_COUNT;

    for (threadCount = 1; threadCount <= THREAD_COUNT; threadCount++) {
      args[threadCount-1] = (struct arg_struct) {
        .trueDigitCount = trueDigitCount,
        .digits = digits,
        .output = output,
        .start = segmentSize * (threadCount - 1),
        .end = threadCount == THREAD_COUNT ? trueDigitCount : segmentSize * threadCount
      };
      pthread_create(&threads[threadCount-1], NULL, thread, (void *) &args[threadCount-1]);
    }

    for (int i = 0; i < threadCount; i++) {
      pthread_join(threads[i], NULL);
    }

    memcpy(digits, output, trueDigitCount * sizeof(int));
  }
  printf("Part 1: ");
  printDigits(digits, 8, 0);
  printf("Part 2 (offset %d): ", offset);
  printDigits(digits, 8, offset);
  free(digits);
}
