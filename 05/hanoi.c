#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct stack {
    char content[50];
    int top;
};

struct stack* createStack() {
    struct stack *s = (struct stack*) malloc(sizeof(struct stack));
    s -> top = -1;
    return s;
}

void pushBottom(struct stack *s, char value) {
    for (int i = s->top; i >= 0; i--) {
        s->content[i+1] = s->content[i];
    }
    s->top++;
    s->content[0] = value;
}

void push(struct stack *s, char value) {
    s->content[++s->top] = value;
}

char pop(struct stack* s) {
    return s->content[s->top--];
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

  struct stack *stacks[10];
  for (int i = 0; i < 10; i++) stacks[i] = createStack();

  int mode = 0;

  char line[50];
  while(fgets(line, 50, fp)) {
    int length = strlen(line);

    if (mode == 0) {
        // Stack def
        if (length == 1) {
          mode = 1;
          continue;
        }
        
        int c = 0;
        while (c < length) {
          if (line[c] == '[') {
            pushBottom(stacks[(c+1) / 4], line[c+1]);
          }
          c += 4;
        }
        
    } else {
        // Moves
        int count, from, to;
        sscanf(line, "move %d from %d to %d", &count, &from, &to);
        for (int i = 0; i < count; i++) {
            push(stacks[to-1], pop(stacks[from-1]));
        }
    }
  }

  printf("Part 1: ");
  for (int i = 0; i < 10; i++) {
    printf("%c", stacks[i]->content[stacks[i]->top]);
  }
  printf("\n");

  fclose(fp);
}