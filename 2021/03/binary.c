#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int setBit(int num, int index) {
  return num | (1 << index);
}

void printNumber(char digits[], int n) {
  for (int i = 0; i < n; i++) printf("%d", digits[i]);
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

  char numbers[1024][16];
  int numLength = 0;

  char line[16];
  int numCount = 0;
  while(fgets(line, 16, fp)) {
    if (!numLength) numLength = strlen(line) - 1;

    for (int c = 0; c < strlen(line) - 1; c++) {
      numbers[numCount][c] = line[c] - '0';
    }
    numCount++;
  }
  fclose(fp);

  int gamma = 0;
  int epsilon = 0;

  for (int c = 0; c < numLength; c++) {
    int sum = 0;
    for (int r = 0; r < numCount; r++) {
      sum += numbers[r][c];
    }
    if(sum > numCount / 2) {
      gamma = setBit(gamma, numLength - 1 - c);
    } else {
      epsilon = setBit(epsilon, numLength - 1 - c);
    }
  }

  printf("Part 1: %d\n", gamma * epsilon);

  int oxygenCandidates[1024];
  int oxygenCandidateCount = numCount;
  for (int i = 0; i < numCount; i++) oxygenCandidates[i] = 1;

  for (int c = 0; c < numLength; c++) {
    int sum = 0;
    for (int r = 0; r < numCount; r++) {
      if (oxygenCandidates[r]) sum += numbers[r][c];
    }
    if(sum >= oxygenCandidateCount - sum) {
      for (int r = 0; r < numCount; r++) {
        if (oxygenCandidates[r] && !numbers[r][c]) {
          oxygenCandidates[r] = 0;
          oxygenCandidateCount--;
        }
      }
    } else {
      for (int r = 0; r < numCount; r++) {
        if (oxygenCandidates[r] && numbers[r][c]) {
          oxygenCandidates[r] = 0;
          oxygenCandidateCount--;
        }
      }
    }
    if (oxygenCandidateCount == 1) break;
  }

  int oxygen = 0;
  for (int cand = 0; cand < numCount; cand++) {
    if (oxygenCandidates[cand]) {
      for (int c = 0; c < numLength; c++) {
        if (numbers[cand][c]) {
          oxygen = setBit(oxygen, numLength - 1 - c);
        }
      }
      break;
    }
  }

  int co2Candidates[1024];
  int co2CandidateCount = numCount;
  for (int i = 0; i < numCount; i++) co2Candidates[i] = 1;

  for (int c = 0; c < numLength; c++) {
    int sum = 0;
    for (int r = 0; r < numCount; r++) {
      if (co2Candidates[r]) sum += numbers[r][c];
    }
    if(sum >= co2CandidateCount - sum) {
      for (int r = 0; r < numCount; r++) {
        if (co2Candidates[r] && numbers[r][c]) {
          co2Candidates[r] = 0;
          co2CandidateCount--;
        }
      }
    } else {
      for (int r = 0; r < numCount; r++) {
        if (co2Candidates[r] && !numbers[r][c]) {
          co2Candidates[r] = 0;
          co2CandidateCount--;
        }
      }
    }
    if (co2CandidateCount == 1) break;
  }

  int co2 = 0;
  for (int cand = 0; cand < numCount; cand++) {
    if (co2Candidates[cand]) {
      for (int c = 0; c < numLength; c++) {
        if (numbers[cand][c]) {
          co2 = setBit(co2, numLength - 1 - c);
        }
      }
      break;
    }
  }

  printf("Part 2: %d\n", oxygen * co2);
}
