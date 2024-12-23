#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONLIMIT 16

struct node {
  char name[3];
  struct node* connections[CONLIMIT];
  int count;
};

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

  int part1 = 0;

  for (int i = 0; i < nodeCount; i++) {
    struct node n1 = nodes[i];

    for (int j = i+1; j < nodeCount; j++) {
      struct node n2 = nodes[j];

      for (int k = j+1; k < nodeCount; k++) {
        struct node n3 = nodes[k];

        int n1n2 = 0;
        int n1n3 = 0;
        for (int c = 0; c < n1.count; c++) {
          if (strcmp(n2.name, n1.connections[c]->name) == 0) {
            n1n2 = 1;
          } else if (strcmp(n3.name, n1.connections[c]->name) == 0) {
            n1n3 = 1;
          }
        }
        int n2n3 = 0;
        for (int c = 0; c < n2.count; c++) {
          if (strcmp(n3.name, n2.connections[c]->name) == 0) {
            n2n3 = 1;
          }
        }
        
        if (n1n2 && n1n3 && n2n3) {
          if (n1.name[0] == 't' || n2.name[0] == 't' || n3.name[0] == 't') part1++;
        }
      }
    }
  }

  printf("Part 1: %d\n", part1);
}
