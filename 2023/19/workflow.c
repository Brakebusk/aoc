#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct rule {
  char variable;
  char operator;
  int value;
  char destination[8];
};

struct workflow {
  char name[8];
  struct rule rules[16];
  int ruleCount;
};

struct part {
  int x, m, a, s;
};

void printWorkflow(struct workflow flow) {
  printf("Name: '%s'\n", flow.name);
  for (int r = 0; r < flow.ruleCount; r++) {
    struct rule selRule = flow.rules[r];

    printf("Var '%c' op '%c' val '%d' dest '%s'\n", selRule.variable, selRule.operator, selRule.value, selRule.destination);
  }
  printf("\n");
}

int findWorkflow(struct workflow flows[600], int workflowCount, char *needle) {
  for (int w = 0; w < workflowCount; w++) {
    if (strcmp(flows[w].name, needle) == 0) {
      return w;
    }
  }
  return -1;
}

int getRating(struct part p) {
  return p.x + p.m + p.a + p.s;
}

int testCondition(int variableValue, char operator, int conditionValue) {
  if (operator == '>') {
    return variableValue > conditionValue;
  } else if (operator == '<') {
    return variableValue < conditionValue;
  }
  printf("Unknown operator '%c'\n", operator);
  exit(EXIT_FAILURE);
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

  struct workflow workflows[600] = {0};
  int workflowCount = 0;

  struct part parts[256];
  int partCount = 0;

  int mode = 0;
  char line[64];
  while(fgets(line, 64, fp)) {
    if (strlen(line) == 1) {
      mode = 1;
      continue;
    }
    if (mode == 0) {
      char *rest = line;
      char *lineSegment = strtok_r(rest, "{", &rest);
      strcpy(workflows[workflowCount].name, lineSegment);
      while ((lineSegment = strtok_r(rest, ",", &rest))) {
        char *relevant = strtok(lineSegment, "}");

        char *left = strtok(relevant, ":");
        char *right = strtok(NULL, ":");

        int ruleCount = workflows[workflowCount].ruleCount;
        if (right) {
          strcpy(workflows[workflowCount].rules[ruleCount].destination, right);

          workflows[workflowCount].rules[ruleCount].variable = left[0];
          workflows[workflowCount].rules[ruleCount].operator = left[1];
          workflows[workflowCount].rules[ruleCount].value = atoi(&left[2]);
        } else {
          strcpy(workflows[workflowCount].rules[ruleCount].destination, left);
        }

        workflows[workflowCount].ruleCount++;
      }
      workflowCount++;
    } else {
      char *rest = &line[1];
      char *lineSegment;
      while ((lineSegment = strtok_r(rest, ",", &rest))) {
        char *relevant = strtok(lineSegment, "}");
        switch (relevant[0]) {
          case 'x':
            parts[partCount].x = atoi(&relevant[2]);
            break;
          case 'm':
            parts[partCount].m = atoi(&relevant[2]);
            break;
          case 'a':
            parts[partCount].a = atoi(&relevant[2]);
            break;
          case 's':
            parts[partCount].s = atoi(&relevant[2]);
            break;
          default:
            printf("Unknown variable '%c'\n", relevant[0]);
            exit(EXIT_FAILURE);
        }
      }
      partCount++;
    }
  }
  fclose(fp);

  int startFlow = findWorkflow(workflows, workflowCount, "in");
  if (startFlow == -1) {
    printf("Could not find start workflow\n");
    exit(EXIT_FAILURE);
  }

  int part1 = 0;

  for (int p = 0; p < partCount; p++) {
    struct part selPart = parts[p];
    int currentFlow = startFlow;
    while (currentFlow > -1) {
      for (int r = 0; r < workflows[currentFlow].ruleCount; r++) {
        struct rule selRule = workflows[currentFlow].rules[r];

        if (!selRule.operator) {
          if (strcmp(selRule.destination, "R") == 0) {
            currentFlow = -1;
            break;
          } else if (strcmp(selRule.destination, "A") == 0) {
            part1 += getRating(selPart);
            currentFlow = -1;
            break;
          } else {
            currentFlow = findWorkflow(workflows, workflowCount, selRule.destination);
            break;
          }
        } else {
          int conditionMet = 0;
          switch (selRule.variable) {
            case 'x':
              if (testCondition(selPart.x, selRule.operator, selRule.value)) {
                conditionMet = 1;
              }
              break;
            case 'm':
              if (testCondition(selPart.m, selRule.operator, selRule.value)) {
                conditionMet = 1;
              }
              break;
            case 'a':
              if (testCondition(selPart.a, selRule.operator, selRule.value)) {
                conditionMet = 1;
              }
              break;
            case 's':
              if (testCondition(selPart.s, selRule.operator, selRule.value)) {
                conditionMet = 1;
              }
              break;
            default:
              printf("Unknown variable '%c'\n", selRule.variable);
              exit(EXIT_FAILURE);
          }
          if (conditionMet) {
            if (strcmp(selRule.destination, "R") == 0) {
              currentFlow = -1;
              break;
            } else if (strcmp(selRule.destination, "A") == 0) {
              part1 += getRating(selPart);
              currentFlow = -1;
              break;
            } else {
              currentFlow = findWorkflow(workflows, workflowCount, selRule.destination);
              break;
            }
          }
        }
      }
    }
  }

  printf("Part 1: %d\n", part1);
}
