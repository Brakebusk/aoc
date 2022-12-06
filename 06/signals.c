#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int OFFSET = 97;
const int MARKER_LENGHT = 4;

void push(char *buffer, char value) {
  for (int i = MARKER_LENGHT; i >= 0; i--) buffer[i+1] = buffer[i];
  buffer[0] = value;
}

int checkBuffer(char *buffer) {
  char seen[26];
  memset(seen, 0, sizeof(char) * 26);
  for (int i = 0; i < MARKER_LENGHT; i++) {
    if (seen[buffer[i] - OFFSET]) return 0;
    seen[buffer[i] - OFFSET] = 1;
  }
  return 1;
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

  int index = 0;
  
  char next;
  char buffer[MARKER_LENGHT];
  memset(buffer, 0, sizeof(char) * MARKER_LENGHT);

  while ((next = fgetc(fp)) != EOF)
  {
    push(buffer, next);

    index++;
    if (checkBuffer(buffer)) break;
  }
  
  printf("Part 1: %d\n", index);

  fclose(fp);
}