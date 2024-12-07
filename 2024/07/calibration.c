#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct equation {
  long long result;
  long long components[16];
  int componentCount;
};

long long raise(long long x, long long y) {
  long long value = 1;
  for (long long i = 0; i < y; i++) {
    value *= x;
  }
  return value;
}

char* getBaseRep(int n, int base) {
    if (n <= 0) {
        char* str = (char*)malloc(2);
        str[0] = '0';
        str[1] = '\0';
        return str;
    };

    int size = 0;
    int temp = n;
    
    while (temp > 0) {
        size++;
        temp /= base;
    }

    char* baseN = (char*)malloc(size + 1);
    baseN[size] = '\0';

    for (int i = size - 1; i >= 0; i--) {
        baseN[i] = (n % base) + '0';
        n /= base;
    }

    return baseN;
}


long long test(struct equation e, int power) {
  int slots = e.componentCount - 1;
  
  for (int p = 0; p < raise(power, slots); p++) {
    long long scratch = e.components[0];
    char *str = getBaseRep(p, power);
    int l = strlen(str);
    
    for (int b = 0; b < slots; b++) {
      int bv = l-b-1 < 0 ? 0 : str[l-b-1] - '0';
      if (bv == 0) {
        scratch += e.components[b+1];
      } else if (bv == 1) {
        scratch *= e.components[b+1];
      } else if (bv == 2) {
        char str[100];
        sprintf(str, "%lld%lld", scratch, e.components[b+1]);
        scratch = atoll(str);
      }
    }
    free(str);
    
    if (scratch == e.result) return e.result;
  }
  
  return 0;
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
  long long part2 = 0;
  for (int i = 0; i < equationCount; i++) {
    struct equation e = equations[i];
    part1 += test(e, 2);    
    part2 += test(e, 3);
  }
  printf("Part 1: %lld\n", part1);
  printf("Part 2: %lld\n", part2);
}
