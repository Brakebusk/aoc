#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "z3/z3.h"

#define MAX_QUEUE_LENGTH 100000
#define MAX_MEMO 1024

// Compile: clang spam.c -lz3 -o spam

uint16_t setBit(uint16_t num, int index) {
  return num | (1 << index);
}

struct machine {
  uint16_t goal;
  int joltages[16];
  int jc;
  uint16_t buttonMasks[16];
  int bc;
};

struct test {
  uint16_t state;
  int button, depth;
};

static Z3_ast makeSum(Z3_context ctx, Z3_ast zero, Z3_ast *terms, int count) {
  if (count == 0) return zero;
  if (count == 1) return terms[0];
  return Z3_mk_add(ctx, count, terms);
}

struct queue {
  struct test items[MAX_QUEUE_LENGTH];
  int front, rear, length;
};

void queuePush(struct queue *q, struct test item) {
  if (q->length >= MAX_QUEUE_LENGTH) {
    printf("Queue overflow!\n");
    exit(EXIT_FAILURE);
  }
  q->items[q->rear] = item;
  q->rear = (q->rear + 1) % MAX_QUEUE_LENGTH;
  q->length++;
}

struct test queueGet(struct queue *q) {
  if (q->length <= 0) {
    printf("Queue underflow!\n");
    exit(EXIT_FAILURE);
  }
  struct test item =  q->items[q->front];
  q->front = (q->front + 1) % MAX_QUEUE_LENGTH;
  q->length--;
  return item;
}

int hasSeen(uint16_t seen[MAX_MEMO], int sc, uint16_t state) {
  for (int i = 0; i < sc; i++) if (seen[i] == state) return 1;
  return 0;
}

int findCombo(struct machine m) {
  struct queue *q = malloc(sizeof(struct queue));
  q->front = 0;
  q->rear = 0;
  q->length = 0;
  uint16_t seen[MAX_MEMO];
  int sc = 0;

  for (int b = 0; b < m.bc; b++) {
    struct test t;
    t.depth = 1;
    t.button = b;
    t.state = 0;
    queuePush(q, t);
  }

  while (q->length) {
    struct test t = queueGet(q);
    
    uint16_t state = t.state ^ m.buttonMasks[t.button];

    if (state == m.goal) {
      free(q);
      return t.depth;
    }

    if (hasSeen(seen, sc, state)) {
      continue;
    }
    if (sc >= MAX_MEMO) {
      printf("Exceeded memo limit!\n");
      exit(EXIT_FAILURE);
    }
    seen[sc++] = state;
    
    for (int b = 0; b < m.bc; b++) {
      struct test newTest;
      newTest.depth = t.depth + 1;
      newTest.button = b;
      newTest.state = state;
      queuePush(q, newTest);
    }
  }
  
  free(q);
  return 0;
}

int findJoltageMin(struct machine *m) {
  Z3_config cfg = Z3_mk_config();
  Z3_context ctx = Z3_mk_context(cfg);
  Z3_del_config(cfg);

  Z3_optimize opt = Z3_mk_optimize(ctx);
  Z3_optimize_inc_ref(ctx, opt);

  Z3_sort intSort = Z3_mk_int_sort(ctx);
  Z3_ast zero = Z3_mk_int(ctx, 0, intSort);

  Z3_ast buttonVars[16];
  for (int b = 0; b < m->bc; b++) {
    char name[16];
    snprintf(name, sizeof(name), "b_%d", b);
    Z3_symbol sym = Z3_mk_string_symbol(ctx, name);
    buttonVars[b] = Z3_mk_const(ctx, sym, intSort);
    Z3_optimize_assert(ctx, opt, Z3_mk_ge(ctx, buttonVars[b], zero));
  }

  for (int j = 0; j < m->jc; j++) {
    Z3_ast terms[16];
    int tc = 0;
    for (int b = 0; b < m->bc; b++) {
      if (m->buttonMasks[b] & (1 << j)) {
        terms[tc++] = buttonVars[b];
      }
    }
    Z3_ast target = Z3_mk_int(ctx, m->joltages[j], intSort);
    Z3_optimize_assert(ctx, opt, Z3_mk_eq(ctx, makeSum(ctx, zero, terms, tc), target));
  }

  Z3_ast totalPresses = makeSum(ctx, zero, buttonVars, m->bc);
  Z3_optimize_minimize(ctx, opt, totalPresses);

  Z3_optimize_check(ctx, opt, 0, NULL);

  Z3_model model = Z3_optimize_get_model(ctx, opt);
  Z3_model_inc_ref(ctx, model);

  int presses = 0;
  for (int b = 0; b < m->bc; b++) {
    Z3_ast value;
    Z3_model_eval(ctx, model, buttonVars[b], 1, &value);
    int val;
    Z3_get_numeral_int(ctx, value, &val);
    presses += val;
  }

  Z3_model_dec_ref(ctx, model);
  Z3_optimize_dec_ref(ctx, opt);
  Z3_del_context(ctx);

  return presses;
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

  struct machine machines[256];
  int mc = 0;

  char line[256];
  while(fgets(line, 256, fp)) {
    char *token = strtok(line, " ");

    machines[mc].jc = 0;
    machines[mc].bc = 0;

    do {
      if (token[0] == '[') {
        machines[mc].goal = 0;
        for (int c = 1; c < strlen(token) - 1; c++) {
          if (token[c] == '#') {
            machines[mc].goal = setBit(machines[mc].goal, c-1);
          }
        }
      } else if (token[0] == '(') {
        char *button, *rest = &token[1];
        uint16_t buttonMask = 0;
        while ((button = strtok_r(rest, ",)", &rest))) {
          buttonMask = setBit(buttonMask, atoi(button));
        }
        machines[mc].buttonMasks[machines[mc].bc++] = buttonMask;
      } else if (token[0] == '{') {
        char *joltage, *rest = &token[1];
        while ((joltage = strtok_r(rest, ",}\n", &rest))) {
          machines[mc].joltages[machines[mc].jc++] = atoi(joltage);
        }
      }
    } while ((token = strtok(NULL, " ")));
    mc++;
  }
  fclose(fp);

  int part1 = 0;
  for (int i = 0; i < mc; i++) part1 += findCombo(machines[i]);
  printf("Part 1: %d\n", part1);

  int part2 = 0;
  for (int i = 0; i < mc; i++) part2 += findJoltageMin(&machines[i]);
  printf("Part 2: %d\n", part2);
}
