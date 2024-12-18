#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long mod(long long a, long long b) {
  return (a % b + b) % b;
}

long long getCombo(long long operand, long long a, long long b, long long c) {
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
      printf("Invalid operand: '%lld'\n", operand);
      exit(EXIT_FAILURE);
  }
}

int runProgram(int program[20], int length, long long initialA, long long initialB, long long initialC, int mode) {
  if (mode == 2) printf("%lld ", initialA);
  long long a = initialA, b = initialB, c = initialC;
  int ip = 0;
  int prints = 0;
  int outBuffer[20] = {0};
  int bufferLength = 0;

  do {
    int opcode = program[ip];
    long long operand = program[ip+1];
    long long combo = getCombo(operand, a, b, c);

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
        printf("%lld", mod(combo, 8));
        if (mode == 2) {
          outBuffer[bufferLength] = mod(combo, 8);
          if (++bufferLength > length) return 0;
        }
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

  printf(" %d", bufferLength);
  for (int i = 0; i < length; i++ ) {
    if (program[i] != outBuffer[i]) return 0;
  }
  return length == bufferLength;
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

  long long a = 0, b = 0, c = 0;
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
        a = atoll(relevant);
        break;
      case 1:
        b = atoll(relevant);
        break;
      case 2:
        c = atoll(relevant);
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

  printf("Part 1: ");
  runProgram(program, length, a, b, c, 1);
  printf("\n");

  printf("                ");
  for (int i = 0; i < length; i++) {
    if (i) printf(",");
    printf("%d", program[i]);
  }
  printf("\n");
  long long test = 0b100101010110100100110000111011010110111010110100;
  while (runProgram(program, length, test, b, c, 2) == 0) {
    printf("\n");
    test+=1;
  }
  printf("\nPart 2: %lld\n", test);
}
