#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OBJECT_COUNT 3000

struct object {
  char name[8];
  int orbits;
  int direct_deps[64];
  int depCount;
};

void printDeps(struct object (*objects), int index) {
  struct object sel = objects[index];
  printf("%s has children [", sel.name);
  for (int d = 0; d < sel.depCount; d++) printf(d > 0 ? ", %s" : "%s", objects[sel.direct_deps[d]].name);
  printf("]\n");

  for (int d = 0; d < sel.depCount; d++) printDeps(objects, sel.direct_deps[d]);
}

int totalIndirectOrbitCount(struct object (*objects), int index, int depth) {
  struct object sel = objects[index];

  int sum = depth;
  for (int d = 0; d < sel.depCount; d++) {
    sum += totalIndirectOrbitCount(objects, sel.direct_deps[d], depth + 1);
  }
  return sum;
}

int distance(struct object (*objects), int selIndex, int fromIndex, int currentDistance, char *goal) {
  struct object sel = objects[selIndex];

  if (strcmp(sel.name, goal) == 0) return currentDistance;

  for (int d = 0; d < sel.depCount; d++) {
    int dep = sel.direct_deps[d];
    if (dep != fromIndex) {
      int dist = distance(objects, dep, selIndex, currentDistance + 1, goal);
      if (dist != -1) return dist;
    }
  }
  if (sel.orbits != fromIndex) return distance(objects, sel.orbits, selIndex, currentDistance + 1, goal);
  return -1;
}

int findObjectByName(struct object (*objects), int objectCount, char *name) {
  for (int i = 0; i < objectCount; i++) {
    if (strcmp(name, objects[i].name) == 0) return i;
  }
  return -1;
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

  struct object (*objects) = malloc(sizeof(struct object[MAX_OBJECT_COUNT]));
  int objectCount = 0;

  char line[10];
  while(fgets(line, 10, fp)) {
    char parentName[8], childName[8];
    char *token = strtok(line, ")");
    strcpy(parentName, token);
    token = strtok(NULL, "\n");
    strcpy(childName, token);

    int childId = findObjectByName(objects, objectCount, childName);
    if (childId == -1) {
      childId = objectCount++;
      strcpy(objects[childId].name, childName);
      objects[childId].depCount = 0;
    }

    int parentId = findObjectByName(objects, objectCount, parentName);
    if (parentId == -1) {
      parentId = objectCount++;
      strcpy(objects[parentId].name, parentName);
      objects[parentId].depCount = 0;
    }
    objects[childId].orbits = parentId;
    objects[parentId].direct_deps[objects[parentId].depCount++] = childId;
  }
  fclose(fp);

  int selected = findObjectByName(objects, objectCount, "COM");
  printf("Part 1: %d\n", totalIndirectOrbitCount(objects, selected, 0));

  int you = findObjectByName(objects, objectCount, "YOU");
  printf("Part 2: %d\n", distance(objects, you, -1, 0, "SAN") - 2);

  free(objects);
}
