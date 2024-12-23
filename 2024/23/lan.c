#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONLIMIT 16

struct node {
  char name[3];
  struct node* connections[CONLIMIT];
  int count;
};

char largestGroup[32][3];

int compareStrings(const void *a, const void *b) {
    const char *str1 = (const char *)a;
    const char *str2 = (const char *)b;
    return strcmp(str1, str2);
}

int group(struct node nodes[550], int nodeCount, char present[32][3], int pCount, int progress, int depth, int requirePrefix) {
  if (depth == 0) {
    memcpy(largestGroup, present, 32*3);
    int tprefix = 0;
    if (requirePrefix) {
      for (int p = 0; p < pCount; p++) {
        if (present[p][0] == 't') {
          tprefix = 1;
          break;
        }
      }
    }
    return requirePrefix ? tprefix : 1;
  } else {
    int count = 0;

    for (int i = progress; i < nodeCount; i++) {
      struct node n = nodes[i];

      int connectedToAllPresent = 1;
      for (int p = 0; p < pCount; p++) {
        int found = 0;
        for (int c = 0; c < n.count; c++) {
          if (strcmp(n.connections[c]->name, present[p]) == 0) {
            found = 1;
            break;
          }
        }
        if (!found) {
          connectedToAllPresent = 0;
          break;
        }
      }

      if (connectedToAllPresent) {
        strcpy(present[pCount], n.name);
        count += group(nodes, nodeCount, present, pCount+1, i+1, depth-1, requirePrefix);
      }
    }

    return count;
  }
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

  struct node nodes[550];
  memset(nodes, 0, sizeof(struct node) * 550);
  int nodeCount = 0;

  char line[8];
  while(fgets(line, 8, fp)) {
    char a[3], b[3];
    sscanf(line, "%2s-%2s", a, b);

    int ia, ib;
    for (ia = 0; ia <= nodeCount; ia++) {
      if (ia == nodeCount) break;
      if (strcmp(a, nodes[ia].name) == 0) break;
    }
    if (ia == nodeCount) {
      strcpy(nodes[nodeCount++].name, a);
    }

    for (ib = 0; ib <= nodeCount; ib++) {
      if (ib == nodeCount) break;
      if (strcmp(b, nodes[ib].name) == 0) break;
    }
    if (ib == nodeCount) {
      strcpy(nodes[nodeCount++].name, b);
    }

    if (nodes[ia].count == CONLIMIT || nodes[ib].count == CONLIMIT) {
      printf("Too many connections!\n");
      exit(EXIT_FAILURE);
    }
    
    nodes[ia].connections[nodes[ia].count++] = &nodes[ib];
    nodes[ib].connections[nodes[ib].count++] = &nodes[ia];
  }
  fclose(fp);

  char present[32][3];
  printf("Part 1: %d\n", group(nodes, nodeCount, present, 0, 0, 3, 1));

  for (int i = 4; i < 15; i++) {
    int found = group(nodes, nodeCount, present, 0, 0, i, 0);
    if (found == 1) {
      qsort(largestGroup, i, 3, compareStrings);
      printf("Part 2: ");
      for (int f = 0; f < i; f++) {
        if (f > 0) printf(",");
        printf("%s", largestGroup[f]);
      }
      printf("\n");
      break;
    }
  }
}
