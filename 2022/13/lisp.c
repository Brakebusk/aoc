#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 30
#define MAX_NESTING 5

int get(int *nested_arr, int level, int i) {
  return nested_arr[level * MAX_LEN + i];
}

void set(int *nested_arr, int level, int i, int value) {
  nested_arr[level * MAX_LEN + i] = value;
}

void print(int *nested_arr) {
  printf("[");
  for (int l = 0; l < MAX_NESTING; l++) {
    printf("[");
    for (int i = 0; i < MAX_LEN; i++) {
      printf(" %d ", get(nested_arr, l, i));
    }
    printf("]\n");
  }
  printf("]\n");
}

int compare(int *list1, int *list2) {
  print(list1);
  return 0;
}

int* parseLine(char line[MAX_LEN]) {
  line[strlen(line) - 1] = '\0';

  // Split the input string into tokens using '[', ']', and ',' as delimiters
  char* token = strtok(line, "[], ");

  // Store the tokens in a nested array
  int* nested_arr = malloc(MAX_NESTING * MAX_LEN * sizeof(int));
  for (int i = 0; i < MAX_NESTING; i++) {
    for (int j = 0; j < MAX_LEN; j++) {
      set(nested_arr, i, j, -1);
    }
  }

  int cur_level = 0;
  int num_elems[MAX_NESTING];
  num_elems[0] = 0;

  while (token != NULL) {
    // If the token is an opening bracket, go to the next nested level
    if (strcmp(token, "[") == 0) {
      cur_level++;
      num_elems[cur_level] = 0;
    }
    // If the token is a closing bracket, go back to the previous nested level
    else if (strcmp(token, "]") == 0) {
      cur_level--;
    }
    // Otherwise, store the token as an integer in the current nested level
    else {
      printf("Settign %d %d to %d\n", cur_level, num_elems[cur_level], atoi(token));
      set(nested_arr, cur_level, num_elems[cur_level], atoi(token));
      num_elems[cur_level]++;
    }

    // Get the next token
    token = strtok(NULL, "[], ");
  }

  return nested_arr;
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

  // Memory leaks aren't bad if you put your head in the sand
  int* list1;
  int* list2;
  
  char line[200];
  int lineCounter = 0;
  while(fgets(line, 200, fp)) {
    switch (lineCounter++ % 3) {
      case 0:
        list1 = parseLine(line);
        break;
      case 1:
        list2 = parseLine(line);
        break;
      case 2:
        compare(list1, list2);
        break;
      default:
        printf("[SANITY] Invalid line counter %d\n", lineCounter);
        exit(EXIT_FAILURE);
    }
  }
  
  fclose(fp);
}
