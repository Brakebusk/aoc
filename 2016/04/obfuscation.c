#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct letter {
  char code;
  int frequency;
};

int compare(const void *a, const void* b) {
  struct letter letter1 = *(struct letter*)a;
  struct letter letter2 = *(struct letter*)b;

  if (letter1.frequency > letter2.frequency) {
    return -1;
  } else if (letter1.frequency < letter2.frequency) {
    return 1;
  } else return letter1.code - letter2.code;
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

  int part1 = 0;

  char line[100];
  while(fgets(line, 100, fp)) {
    unsigned char name[64];
    int nameLength = 0;
    memset(name, 0, 64);

    char *token = strtok(line, "[");

    struct letter count[26];
    for (int i = 0; i < 26; i++) {
      count[i].code = i + 'a';
      count[i].frequency = 0;
    }
    int id = 0;
    
    for (int i = 0; i < strlen(token); i++) {
      if (token[i] != '-') {
        if (token[i] < 'a') {
          id *= 10;
          id += token[i] - '0';
        } else {
          name[nameLength++] = token[i];
          count[token[i]-'a'].frequency++;
        }
      } else name[nameLength++] = token[i];
    }

    qsort(count, 26, sizeof(struct letter), compare);

    token = strtok(NULL, "]");

    int valid = 1;
    for (int i = 0; i < strlen(token); i++) {
      if (count[i].code != token[i]) {
        valid = 0;
        break;
      }
    }
    if (valid) {
      part1 += id;

      for (int i = 0; i < strlen((char *)name); i++) {
        if (name[i] != '-') {
          name[i] = name[i] + (id % 26);
          if (name[i] > 'z') name[i] -= 26;
        } else name[i] = ' ';

      }
      printf("%d: %s\n", id, name);
    }
  }
  fclose(fp);

  printf("Part 1: %d\n", part1);
}
