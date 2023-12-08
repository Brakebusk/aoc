#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long gcd(long long a, long long b) {
    if (a == 0) {
        return b;
    }
    return gcd(b % a, a);
}

long long lcm(int arr[], int n) {
    long long ans = arr[0];
    for (int i = 1; i < n; i++) {
        ans = (((arr[i] * ans)) / (gcd(arr[i], ans)));
    }
    return ans;
}

struct node {
  unsigned short next[2];
};

struct startingLocation {
  unsigned short location;
  int steps;
};

char letterToNumber(char letter) {
  return letter - 'A';
}

unsigned short nameToCoordinate(char name[]) {
  char n1 = letterToNumber(name[0]);
  char n2 = letterToNumber(name[1]);
  char n3 = letterToNumber(name[2]);

  unsigned short result = ((n1 & 0b00011111) << 10) | ((n2 & 0b00011111) << 5) | (n3 & 0b00011111);
  return result;
}

int isStartingLocation(unsigned short location) {
  return (location & 0b0000000000011111) == 0;
}

int isGoal(unsigned short location) {
  return (location & 0b0000000000011111) == 0b0000000000011001;
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

  int steps[512];
  int stepCount = 0;

  struct node grid[27000];

  struct startingLocation locations[16];
  int locationCount = 0;

  char line[512];
  int lineCount = 0;
  while(fgets(line, 512, fp)) {
    if (lineCount == 0) {
      for (int i = 0; i < strlen(line) - 1; i++) steps[stepCount++] = line[i] == 'R';
    } else if (lineCount > 1) {
      char from[4] = {0}, left[4] = {0}, right[4] = {0};
      sscanf(line, "%3s = (%3s, %3s)", from, left, right);
      unsigned short fromCoordinate = nameToCoordinate(from);
      grid[fromCoordinate] = (struct node) {
        .next = {nameToCoordinate(left), nameToCoordinate(right)}
      };
      if (isStartingLocation(fromCoordinate)) {
        locations[locationCount++] = (struct startingLocation) {
          .location = fromCoordinate,
          .steps = 0
        };
      }
    }
    lineCount++;
  }
  fclose(fp);

  int step = 0;
  unsigned short location = 0;
  unsigned short goal = nameToCoordinate("ZZZ");
  while (location != goal) {
    int nextStep = steps[step++ % stepCount];
    location = grid[location].next[nextStep];
  }
  printf("Part 1: %d\n", step);


  int numSteps[16];
  for (int i = 0; i < locationCount; i++) {
    while (!isGoal(locations[i].location)) {
      locations[i].location = grid[locations[i].location].next[steps[locations[i].steps++ % stepCount]];
    }
    numSteps[i] = locations[i].steps;
  }
  printf("Part 2: %lld\n", lcm(numSteps, locationCount));
}
