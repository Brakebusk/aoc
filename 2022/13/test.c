#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Maximum length of the input string
#define MAX_LEN 1000

// Maximum number of nested levels
#define MAX_NESTING 10

int main() {
  // Read the input string
  char input_str[MAX_LEN];
  printf("Enter the nested list as a string: ");
  fgets(input_str, MAX_LEN, stdin);

  // Remove the newline character at the end of the string
  input_str[strlen(input_str) - 1] = '\0';

  // Split the input string into tokens using '[', ']', and ',' as delimiters
  char* token = strtok(input_str, "[], ");

  // Store the tokens in a nested array
  int nested_arr[MAX_NESTING][MAX_LEN];
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
      nested_arr[cur_level][num_elems[cur_level]] = atoi(token);
      num_elems[cur_level]++;
    }

    // Get the next token
    token = strtok(NULL, "[], ");
  }

  // Print the nested array
  printf("\nNested array:\n");
  int i, j;
  for (i = 0; i <= cur_level; i++) {
    printf("[");
    for (j = 0; j < num_elems[i]; j++) {
      printf("%d, ", nested_arr[i][j]);
    }
    printf("]\n");
  }
}

 
