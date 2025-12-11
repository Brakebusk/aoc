#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct device {
  char name[4];
  int outputs[32];
  int oc;
};

int findDevice(struct device devices[600], int dc, char *name) {
  for (int d = 0; d < dc; d++) {
    if (strcmp(devices[d].name, name) == 0) return d;
  }
  return -1;
}

long long countPaths(struct device devices[600], long long memo[600][4], int current, int goal, int dacIndex, int visitedDac, int fftIndex, int visitedFft) {
  if (current == goal) return visitedDac && visitedFft;
  
  int memoSubIndex = 2 * visitedDac + visitedFft;
  if (memo[current][memoSubIndex] > -1) return memo[current][memoSubIndex];
  
  long long paths = 0;
  for (int o = 0; o < devices[current].oc; o++) {
    paths += countPaths(devices, memo, devices[current].outputs[o], goal, dacIndex, visitedDac || current == dacIndex, fftIndex, visitedFft || current == fftIndex);
  }
  memo[current][memoSubIndex] = paths;
  return paths;
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

  struct device devices[600];
  memset(devices, 0, sizeof(struct device) * 600);
  int dc = 0;

  char line[128];
  while(fgets(line, 128, fp)) {
    char *token = strtok(line, ":");

    int deviceIndex = findDevice(devices, dc, token);
    if (deviceIndex == -1) {
      strcpy(devices[dc].name, token);
      deviceIndex = dc++;
    }

    while ((token = strtok(NULL, " \n"))) {
      int outputIndex = findDevice(devices, dc, token);
      if (outputIndex == -1) {
        strcpy(devices[dc].name, token);
        outputIndex = dc++;
      }
      devices[deviceIndex].outputs[devices[deviceIndex].oc++] = outputIndex;
    }
  }
  fclose(fp);

  int youIndex = findDevice(devices, dc, "you");
  int svrIndex = findDevice(devices, dc, "svr");
  int outIndex = findDevice(devices, dc, "out");
  int dacIndex = findDevice(devices, dc, "dac");
  int fftIndex = findDevice(devices, dc, "fft");

  long long memo[600][4];
  for (int i = 0; i < 600; i++) for (int v = 0; v < 4; v++) memo[i][v] = -1;

  long long part1 = countPaths(devices, memo, youIndex, outIndex, dacIndex, 1, fftIndex, 1);
  printf("Part 1: %lld\n", part1);

  for (int i = 0; i < 600; i++) for (int v = 0; v < 4; v++) memo[i][v] = -1;

  long long part2 = countPaths(devices, memo, svrIndex, outIndex, dacIndex, 0, fftIndex, 0);
  printf("Part 2: %lld\n", part2);
}
