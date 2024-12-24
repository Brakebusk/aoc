#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NODELIM 512

struct node {
  char name[4];
  char value;
};

// types: 0 => AND, 1 => OR, 2 => XOR
struct gate {
  char a[4], b[4], type, res[4], evaluated;
};

int compareStrings(const void *a, const void *b) {
    const char *str1 = (const char *)a;
    const char *str2 = (const char *)b;
    return strcmp(str1, str2);
}

int findNode(struct node nodes[NODELIM], int nodeCount, char name[4]) {
  for (int i = 0; i < nodeCount; i++) {
    if (strcmp(name, nodes[i].name) == 0) {
      return i;
    }
  }
  return -1;
}

long long setBit(long long num, int index) {
  return num | (1LL << index);
}

long long getValue(struct node nodes[NODELIM], int nodeCount, char prefix) {
  long long value;
  for (int i = 0; i < nodeCount; i++) {
    if (nodes[i].name[0] == prefix && nodes[i].value) {
      int index = atoi(&nodes[i].name[1]);
      value = setBit(value, index);
    }
  }
  return value;
}

void swap(struct gate* a, struct gate* b) {
  char tmp[4];
  strcpy(tmp, a->res);
  strcpy(a->res, b->res);
  strcpy(b->res, tmp);
}

long long run(struct node nodes[NODELIM], int nodeCount, struct gate gates[256], int gateCount) {
  int found = 1;
  while (found) {
    found = 0;

    for (int i = 0; i < gateCount; i++) {
      if (gates[i].evaluated == 0) {
        int a = findNode(nodes, nodeCount, gates[i].a);
        int b = findNode(nodes, nodeCount, gates[i].b);
        if (a > -1 && b > -1) {
          if (nodeCount == NODELIM) {
            printf("Too many nodes\n");
            exit(EXIT_FAILURE);
          }

          found = 1;
          char aVal = nodes[a].value;
          char bVal = nodes[b].value;
          char type = gates[i].type;

          char resVal;
          switch (type) {
            case 0:
              resVal = aVal & bVal;
              break;
            case 1:
              resVal = aVal | bVal;
              break;
            case 2:
              resVal = aVal ^ bVal;
              break;
            default:
              printf("Unknown type eval: '%d'\n", type);
              exit(EXIT_FAILURE);
          }

          strcpy(nodes[nodeCount].name, gates[i].res);
          nodes[nodeCount++].value = resVal;

          gates[i].evaluated = 1;
        }
      }
    }
  }

  return getValue(nodes, nodeCount, 'z');
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
  
  struct node initialNodes[NODELIM];
  int nodeCount = 0;
  struct gate initialGates[256];
  int gateCount = 0;

  char line[32];
  int mode = 0;
  while(fgets(line, 32, fp)) {
    if (strlen(line) <= 1) {
      mode = 1;
    } else {
      if (mode == 0) {
        char name[4];
        int value;
        sscanf(line, "%3s: %d", name, &value);
        strcpy(initialNodes[nodeCount].name, name);
        initialNodes[nodeCount++].value = value;
      } else {
        char *token = strtok(line, "-");
        char *rest = token;
        char *segment;
        int c = 0;
        while ((segment = strtok_r(rest, " ", &rest))) {
          if (c == 0) {
            strcpy(initialGates[gateCount].a, segment);
          } else if (c == 1) {
            if (strcmp(segment, "AND") == 0) {
              initialGates[gateCount].type = 0;
            } else if (strcmp(segment, "OR") == 0) {
              initialGates[gateCount].type = 1;
            } else if (strcmp(segment, "XOR") == 0) {
              initialGates[gateCount].type = 2;
            } else {
              printf("Unknown type: '%s'\n", segment);
              exit(EXIT_FAILURE);
            }
          } else {
            strcpy(initialGates[gateCount].b, segment);
          }
          c++;
        }
        token = strtok(NULL, " ");
        token = strtok(NULL, " \n");
        initialGates[gateCount].evaluated = 0;
        strcpy(initialGates[gateCount++].res, token);
      }
    }
  }
  fclose(fp);


  struct node nodes[NODELIM];
  memcpy(nodes, initialNodes, sizeof(struct node) * NODELIM);
  struct gate gates[256];
  memcpy(gates, initialGates, sizeof(struct gate) * 256);
  printf("Part 1: %lld\n", run(nodes, nodeCount, gates, gateCount));

  long long desiredResult = getValue(initialNodes, nodeCount, 'x') + getValue(initialNodes, nodeCount, 'y');
  for (int a = 0; a < nodeCount; a++) {
    for (int b = a+1; b < nodeCount; b++) {
      for (int c = b+1; c < nodeCount; c++) {
        for (int d = c+1; d < nodeCount; d++) {
          for (int e = d+1; e < nodeCount; e++) {
            for (int f = e+1; f < nodeCount; f++) {
              for (int g = f+1; g < nodeCount; g++) {
                for (int h = g+1; h < nodeCount; h++) {
                  memcpy(nodes, initialNodes, sizeof(struct node) * NODELIM);
                  memcpy(gates, initialGates, sizeof(struct gate) * 256);

                  swap(&gates[a], &gates[b]);
                  swap(&gates[c], &gates[d]);
                  swap(&gates[e], &gates[f]);
                  swap(&gates[g], &gates[h]);

                  if (run(nodes, nodeCount, gates, gateCount) == desiredResult) {
                    char resultNames[8][4];
                    strcpy(resultNames[0], gates[a].res);
                    strcpy(resultNames[1], gates[b].res);
                    strcpy(resultNames[2], gates[c].res);
                    strcpy(resultNames[3], gates[d].res);
                    strcpy(resultNames[4], gates[e].res);
                    strcpy(resultNames[5], gates[f].res);
                    strcpy(resultNames[6], gates[g].res);
                    strcpy(resultNames[7], gates[h].res);
                    qsort(resultNames, 8, 4, compareStrings);

                    printf("Part 2: ");
                    for (int i = 0; i < 8; i++) {
                      if (i > 0) printf(",");
                      printf("%s", resultNames[i]);
                    }
                    printf("\n");
                    exit(EXIT_SUCCESS);
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}
