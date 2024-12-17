#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int mod(int a, int b) {
  return (a % b + b) % b;
}


int getCombo(int operand, int a, int b, int c) {
  switch (operand) {
    case 0:
    case 1:
    case 2:
    case 3:
      return operand;
    case 4:
      return a;
    case 5:
      return b;
    case 6:
      return c;
    case 7:
    default:
      printf("Invalid operand: '%d'\n", operand);
      exit(EXIT_FAILURE);
  }
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

  int a = 0, b = 0, c = 0;
  int program[20];
  int length = 0;

  char line[50];
  int mode = 0;
  while(fgets(line, 512, fp)) {
    char relevant[50];
    char *token = 0;
    while ((token = strtok(token ? NULL : line, " "))) {
      strcpy(relevant, token);
    }
    switch (mode) {
      case 0:
        a = atoi(relevant);
        break;
      case 1:
        b = atoi(relevant);
        break;
      case 2:
        c = atoi(relevant);
        break;
      case 3:
        break;
      case 4:
        token = 0;
        while ((token = strtok(token ? NULL : relevant, ","))) {
          program[length++] = atoi(token);
        }
        break;
    }
    mode++;
  }
  fclose(fp);

  int ip = 0;
  int prints = 0;
  do {
    int opcode = program[ip];
    int operand = program[ip+1];
    int combo = getCombo(operand, a, b, c);

    switch (opcode) {
      case 0: // adv => A = A / 2^combo
        a = a / (combo ? (2 << (combo - 1)) : 1);
        break;
      case 1: // bxl => B = B XOR literal
        b = b ^ operand;
        break;
      case 2: // bst => B = combo % 8
        b = mod(combo, 8);
        break;
      case 3: // jnz => if A != 0 then ip = literal (don't increment by 2)
        if (a != 0) ip = operand - 2;
        break;
      case 4: // bxc => B = B XOR C (ignore operand)
        b = b ^ c;
        break;
      case 5: // out => output combo % 8
        if (prints++) printf(",");
        printf("%d", mod(combo, 8));
        break;
      case 6: // bdv => B = A / 2^combo
        b = a / (combo ? (2 << (combo - 1)) : 1);
        break;
      case 7: // cdv => C = A / 2^combo
        c = a / (combo ? (2 << (combo - 1)) : 1);
        break;
      default:
        printf("Invalid instruction: '%d'\n", opcode);
        exit(EXIT_FAILURE);
    }
  } while ((ip += 2) < length);
  printf("\n");
}
