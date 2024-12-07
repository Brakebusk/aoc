#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct equation {
  long long result;
  long long components[16];
  int componentCount;
};

// Index 0 = LSB
int getBitValue(int num, int index) {
  return (num & (1 << index)) != 0;
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

  struct equation equations[850];
  int equationCount = 0;

  char line[64];
  while(fgets(line, 64, fp)) {
    char *token = strtok(line, ":");
    equations[equationCount].result = atoll(token);
    equations[equationCount].componentCount = 0;
    while ((token = strtok(NULL, " "))) {
      equations[equationCount].components[equations[equationCount].componentCount++] = atoll(token);
    }
    equationCount++;
  }
  fclose(fp);

  long long part1 = 0;
  for (int i = 0; i < equationCount; i++) {
    struct equation e = equations[i];

    int slots = e.componentCount - 1;
    for (int p = 0; p < 2 << (slots-1); p++) {
      long long scratch = e.components[0];
      for (int b = 0; b < slots; b++) {
        if (getBitValue(p, b) == 0) {
          scratch += e.components[b+1];
        } else {
          scratch *= e.components[b+1];
        }
      }
      if (scratch == e.result) {
        part1 += e.result;
        break;
      }
    }
    
  }
  printf("Part 1: %lld\n", part1);
}
