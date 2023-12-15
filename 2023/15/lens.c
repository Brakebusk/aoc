#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int hash(char *sequence, int sequenceLength) {
  int hash = 0;
  for (int c = 0; c < sequenceLength; c++) {
    hash += sequence[c];
    hash *= 17;
    hash = hash % 256;
  }
  return hash;
}

struct lens {
  char label[16];
  int labelLenth;
  int focalLength;
  int hash;
};

struct box {
  struct lens lenses[128];
  int lensCount;
};

void removeLens(struct box *from, char *label) {
  int index = -1;
  for (int i = 0; i < from->lensCount; i++) {
    if (strcmp(from->lenses[i].label, label) == 0) {
      index = i;
      break;
    }
  }
  if (index > -1) {
    for (int i = index; i < from->lensCount - 1; i++) {
      memcpy(&from->lenses[i], &from->lenses[i+1], sizeof(struct lens));
    }
    from->lensCount--;
  }
}

void insertLens(struct box *into, struct lens value) {
  int index = -1;
  for (int i = 0; i < into->lensCount; i++) {
    if (strcmp(into->lenses[i].label, value.label) == 0) {
      index = i;
      break;
    }
  }
  memcpy(&into->lenses[index > -1 ? index : into->lensCount++], &value, sizeof(struct lens));
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

  char line[23000];
  fgets(line, 23000, fp);
  fclose(fp);

  int part1 = 0;

  struct box boxes[256] = {0};

  char *token = 0;
  while ((token = strtok(token ? NULL : line, ","))) {
    int sequenceLength = strlen(token);
    part1 += hash(token, sequenceLength);

    if (token[sequenceLength-1] == '-') {
      char label[16] = {0};
      strncpy(label, token, sequenceLength-1);
      int labelHash = hash(label, sequenceLength-1);
      removeLens(&boxes[labelHash], label);
    } else {
      int foundSep = 0;
      struct lens current = (struct lens) {
        .label = {0},
        .labelLenth = 0
      };
      for (int c = 0; c < sequenceLength; c++) {
        if (token[c] == '=') {
          foundSep = 1;
          continue;
        }
        if (foundSep) {
          current.focalLength = atoi(&token[c]);
          break;
        } else {
          current.label[current.labelLenth++] = token[c];
        }
      }
      current.hash = hash(current.label, current.labelLenth);
      insertLens(&boxes[current.hash], current);
    }
  }

  int part2 = 0;

  for (int b = 0; b < 256; b++) {
    struct box current = boxes[b];
    if (current.lensCount) {
      for (int l = 0; l < current.lensCount; l++) {
        struct lens selectedLens = current.lenses[l];
        part2 += (b + 1) * (l + 1) * selectedLens.focalLength;
      }
    }
  }

  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}
