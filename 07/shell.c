#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct File {
  char name[40];
  int size;
};

struct Node {
  char name[40];
  struct Node* parent;
  struct File* files[20];
  int fileCount;
  struct Node* directories[20];
  int directoryCount;
  int totalSize;
};

struct Node* createNode(char *name, struct Node *parent) {
  struct Node *n = (struct Node*) malloc(sizeof(struct Node));
  strcpy(n->name, name);
  n->fileCount = 0;
  n->directoryCount = 0;
  n->parent = parent;
  return n;
}

void createFile(struct Node *current, char* name, int size) {
  struct File *f = (struct File*) malloc(sizeof(struct File));
  strcpy(f->name, name);
  f->size = size;

  current->files[current->fileCount++] = f;
}

struct Node* navigate(struct Node* current, char *name) {
  if (strcmp(name, "..") == 0) {
    return current->parent;
  } else {
    for (int i = 0; i < current->directoryCount; i++) {
      if (strcmp(current->directories[i]->name, name) == 0) {
        return current->directories[i];
      }
    }
    // Directory does not exist yet
    struct Node* newNode = createNode(name, current);
    current->directories[current->directoryCount++] = newNode;
    return newNode;
  }
}

int calculateDirectorySize(struct Node *current) {
  int totalFilesize = 0;
  for (int i = 0; i < current->fileCount; i++) {
    totalFilesize += current->files[i]->size;
  }

  int childrenSize = 0;
  for (int i = 0; i < current->directoryCount; i++) {
    childrenSize += calculateDirectorySize(current->directories[i]);
  }

  int totalSize = totalFilesize + childrenSize;
  current->totalSize = totalSize;
  return totalSize;
}

int findDirectoriesSmallerThan(struct Node *current, int limit) {
  int childrenSmallerThan = 0;
  for (int i = 0; i < current->directoryCount; i++) {
    childrenSmallerThan += findDirectoriesSmallerThan(current->directories[i], limit);
  }
  return childrenSmallerThan + (current->totalSize > limit ? 0 : current->totalSize);
}

int findSmallestAboveLimit(struct Node* current, int limit) {
  int smallestChildAbove = INT32_MAX;
  for (int i = 0; i < current->directoryCount; i++) {
    int child = findSmallestAboveLimit(current->directories[i], limit);
    if (child > limit && child < smallestChildAbove) smallestChildAbove = child;
  }

  if (current->totalSize > limit && current->totalSize < smallestChildAbove) {
    return current->totalSize;
  }
  return smallestChildAbove;
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

  struct Node* root = createNode("/", NULL);
  struct Node* current;

  char line[40];
  while(fgets(line, 50, fp)) {
    if (line[0] == '$') {
      char command[5] = {0}, param[40] = {0};
      sscanf(line, "$ %s %[^\n]", command, param);

      if (strcmp(command, "cd") == 0) {
        if (strcmp(param, "/") == 0) {
          current = root;
        } else {
          current = navigate(current, param);
        }
      }
    } else {
      // Directory listing
      char first[40] = {0}, second[40] = {0};
      sscanf(line, "%s %[^\n]", first, second);
      if (strcmp(first, "dir") != 0) {
        int size = atoi(first);
        createFile(current, second, size);
      }
    }
  }

  calculateDirectorySize(root);

  printf("Part 1: %d\n", findDirectoriesSmallerThan(root, 100001));

  int mustDelete = 30000000 - (70000000 - root->totalSize);

  printf("Part 2: %d\n", findSmallestAboveLimit(root, mustDelete - 1));

  fclose(fp);
}