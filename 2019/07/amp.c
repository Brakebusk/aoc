#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define MEMORY 1024
#define MAX_PARAMS 3
#define MAX_QUEUE_LENGTH 8
#define DEBUG_LEVEL 1

void debugLog(int level, const char *format, ...) {
    if (DEBUG_LEVEL < level) return;
    char *buffer;
    va_list args;
    va_start(args, format);
    vasprintf(&buffer, format, args);
    va_end(args);

    printf("[DEBUG] %s\n", buffer);
}

void debugMemory(int memory[MEMORY], int from, int to) {
  for (int i = from; i <= to; i++) printf("%d ", memory[i]);
  printf("\n");
}

struct queue {
  int length;
  int items[MAX_QUEUE_LENGTH];
};

void queuePush(struct queue* q, int value) {
  if ((*q).length == MAX_QUEUE_LENGTH) {
    printf("[queuePush] Queue is full!\n");
    exit(EXIT_FAILURE);
  }
  (*q).items[(*q).length++] = value;
}

int queueGet(struct queue* q) {
  if ((*q).length == 0) {
    printf("[queueGet] Queue is empty!\n");
    exit(EXIT_FAILURE);
    return -1;
  }
  int value = (*q).items[0];
  memcpy((*q).items, &(*q).items[1], sizeof(int) * (MAX_QUEUE_LENGTH - 1));
  (*q).length--;
  return value;
}

struct queue* newQueue(int count, ...) {
  struct queue *q = malloc(sizeof(struct queue));

  va_list args;
  va_start(args, count);
  for (int i = 0; i < count; i++) {
    queuePush(q, va_arg(args, int));
  }
  va_end(args);

  return q;
}

int validateAddress(int address, char *trace) {
  if (address < 0 || address >= MEMORY) {
    printf("[%s] accessing invalid memory address %d\n", trace, address);
    exit(EXIT_FAILURE);
  }
  return 1;
}

int getValue(int memory[MEMORY], int address) {
  validateAddress(address, "getValue");
  debugLog(3, "Getting %d (%d)", address, memory[address]);
  return memory[address];
}

void setValue(int *memory, int address, int value) {
  validateAddress(address, "setValue");
  debugLog(3, "Setting %d to %d", address, value);
  memory[address] = value;
}

void parseInstruction(int *opCode, int *parameterModes, int instruction) {
  int relevant = instruction % 100;
  (*opCode) = relevant;
  instruction /= 100;

  int modes = 0;
  while (instruction > 0) {
      relevant = instruction % 10;
      parameterModes[modes++] = relevant;
      instruction /= 10;
  }
}

void getParameters(int memory[MEMORY], int parameterModes[MAX_PARAMS], int instructionPointer, int *dest) {
  for (int p = 0; p < MAX_PARAMS; p++) {
    int memoryLocation = instructionPointer + 1 + p;
    switch (parameterModes[p]) {
      case 0: // Position mode
        dest[p] = getValue(memory, memoryLocation);
        break;
      case 1: // Immediate mode
        dest[p] = memoryLocation;
        break;
      default:
        printf("[getParameters] Invalid parameter mode: %d\n", parameterModes[p]);
        exit(EXIT_FAILURE);
    }
  }
}

int getNextInstructionPointer(int currentIP, int paramCount) {
  return currentIP + paramCount + 1;
}

int runProgram(int sourceCode[MEMORY], struct queue* ioQueue) {
  int memory[MEMORY];
  memcpy(memory, sourceCode, sizeof(int) * MEMORY);

  int instructionPointer = 0;
  int halt = 0;
  while (!halt) {
    int opCode, parameterModes[MAX_PARAMS] = {0}, params[MAX_PARAMS];
    parseInstruction(&opCode, parameterModes, memory[instructionPointer]);
    getParameters(memory, parameterModes, instructionPointer, params);

    debugLog(2, "Running instruction %d", opCode);

    int condition;
    
    switch (opCode) {
      case 1: // ADD: Param 3 = param 1 + param 2
        setValue(memory, params[2], getValue(memory, params[0]) + getValue(memory, params[1]));
        instructionPointer = getNextInstructionPointer(instructionPointer, 3);
        break;
      case 2: // MUL: Param 3 = param 1 * param 2
        setValue(memory, params[2], getValue(memory, params[0]) * getValue(memory, params[1]));
        instructionPointer = getNextInstructionPointer(instructionPointer, 3);
        break;
      case 3: // INP: Param 1 = input
        setValue(memory, params[0], queueGet(ioQueue));
        instructionPointer = getNextInstructionPointer(instructionPointer, 1);
        break;
      case 4: // OUT: Output param 1
        debugLog(1, "Outputting %d", getValue(memory, params[0]));
        queuePush(ioQueue, getValue(memory, params[0]));
        instructionPointer = getNextInstructionPointer(instructionPointer, 1);
        break;
      case 5: // JMPT: jump to param 2 if param 1 != 0
        condition = getValue(memory, params[0]);
        instructionPointer = condition != 0 ? getValue(memory, params[1]) : getNextInstructionPointer(instructionPointer, 2);
        break;
      case 6: // JMPF: jump to param 2 if param 1 == 0
        condition = getValue(memory, params[0]);
        instructionPointer = condition == 0 ? getValue(memory, params[1]) : getNextInstructionPointer(instructionPointer, 2);
        break;
      case 7: // LESS param 3 = param 1 < param 2 ? 1 : 0
        setValue(memory, params[2], getValue(memory, params[0]) < getValue(memory, params[1]));
        instructionPointer = getNextInstructionPointer(instructionPointer, 3);
        break;
      case 8: // EQ param 3 = param 1 == param 2
        setValue(memory, params[2], getValue(memory, params[0]) == getValue(memory, params[1]));
        instructionPointer = getNextInstructionPointer(instructionPointer, 3);
        break;
      case 99:
        halt = 1;
        break;
      default:
        printf("Unknown opCode %d\n", opCode);
        exit(EXIT_FAILURE);
    }
  }

  return memory[0];
}

int* readSourceCode(char *filename) {
  FILE *fp = NULL;
  if ((fp = fopen(filename, "r")) == NULL) {
      printf("[ERROR] Failed to open file %s\n", filename);
      exit(EXIT_FAILURE);
  }

  char line[3000];
  fgets(line, 3000, fp);
  fclose(fp);

  int* memory = malloc(sizeof(int) * MEMORY);
  memset(memory, 0, sizeof(int) * MEMORY);
  
  int currentAddress = 0;
  char *token = 0;
  while ((token = strtok(token ? NULL : line, ","))) {
    memory[currentAddress++] = atoi(token);
  }
  
  return memory;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("[ERROR] Missing parameter <filename>\n");
    exit(EXIT_FAILURE);
  }

  char *filename = argv[1];
  int *program = readSourceCode(filename);

  int sequence[] = {4,3,2,1,0};
  struct queue *ioQueue = newQueue(2, sequence[0], 0);

  for (int s = 0; s < 5; s++) {
    if (s < 4) queuePush(ioQueue, sequence[s+1]);
    runProgram(program, ioQueue);
  }

  free(program);
  free(ioQueue);
}
