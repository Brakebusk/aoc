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

int countPaths(struct device devices[600], int dc, int current, int goal) {
  if (current == goal) return 1;

  int paths = 0;
  for (int o = 0; o < devices[current].oc; o++) {
    paths += countPaths(devices, dc, devices[current].outputs[o], goal);
  }
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

  int startIndex = findDevice(devices, dc, "you");
  int endIndex = findDevice(devices, dc, "out");
  if (startIndex == -1 || endIndex == -1) {
    printf("Missing 'you' or 'out' devices!\n");
    exit(EXIT_FAILURE);
  }

  int part1 = countPaths(devices, dc, startIndex, endIndex);
  printf("Part 1: %d\n", part1);
}
