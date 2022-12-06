#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int OFFSET = 97; // ASCII offset of 'a'

void push(char *buffer, int bufferSize, char value) {
  for (int i = bufferSize; i >= 0; i--) buffer[i+1] = buffer[i];
  buffer[0] = value;
}

int checkBuffer(char *buffer, int bufferSize) {
  char seen[26];
  memset(seen, 0, sizeof(char) * 26);
  for (int i = 0; i < bufferSize; i++) {
    if (seen[buffer[i] - OFFSET]) return 0;
    seen[buffer[i] - OFFSET] = 1;
  }
  return 1;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("[ERROR] Missing parameter <filename> <marker length>\n");
    exit(EXIT_FAILURE);
  }

  char *filename = argv[1];
  int markerLength = atoi(argv[2]);

  FILE *fp = NULL;
  if ((fp = fopen(filename, "r")) == NULL) {
      printf("[ERROR] Failed to open file %s\n", filename);
      exit(EXIT_FAILURE);
  }

  int index = 0;
  
  char next;
  char buffer[markerLength];
  memset(buffer, 0, sizeof(char) * markerLength);

  while ((next = fgetc(fp)) != EOF)
  {
    push(buffer, markerLength, next);

    index++;
    if (index >= markerLength && checkBuffer(buffer, markerLength)) break;
  }
  
  printf("Part X: %d\n", index);

  fclose(fp);
}