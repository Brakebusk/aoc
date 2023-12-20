#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define MEMORY 512
#define MAX_PARAMS 3
#define MAX_QUEUE_LENGTH 8
#define DEBUG 1

void debugLog(const char *format, ...) {
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

int validateAddress(int address, char *trace) {
  if (address < 0 || address >= MEMORY) {
    printf("[%s] accessing invalid memory address %d\n", trace, address);
    exit(EXIT_FAILURE);
  }
  return 1;
}

int getValue(int memory[MEMORY], int address) {
  validateAddress(address, "getValue");
  debugLog("Getting %d (%d)", address, memory[address]);
  return memory[address];
}

void setValue(int *memory, int address, int value) {
  validateAddress(address, "setValue");
  debugLog("Setting %d to %d", address, value);
  memory[address] = value;
}

void parseInstruction(int *opCode, int *parameterModes, int instruction) {
  debugLog("Parsing instruction %d", instruction);
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

int runProgram(int sourceCode[MEMORY], int noun, int verb, struct queue sourceInputQueue) {
  int memory[MEMORY];
  memcpy(memory, sourceCode, sizeof(int) * MEMORY);
  
  struct queue inputQueue;
  memcpy(&inputQueue, &sourceInputQueue, sizeof(struct queue));
 
  if (noun >= 0) memory[1] = noun;
  if (verb >= 0) memory[2] = verb;

  int instructionPointer = 0;
  int halt = 0;
  while (!halt) {
    int opCode, parameterModes[MAX_PARAMS] = {0}, params[MAX_PARAMS];
    parseInstruction(&opCode, parameterModes, memory[instructionPointer]);
    getParameters(memory, parameterModes, instructionPointer, params);

    debugLog("Running instruction %d", opCode);
    
    switch (opCode) {
      case 1:
        setValue(memory, params[2], getValue(memory, params[0]) + getValue(memory, params[1]));
        instructionPointer += 4;
        break;
      case 2:
        setValue(memory, params[2], getValue(memory, params[0]) * getValue(memory, params[1]));
        instructionPointer += 4;
        break;
      case 3:
        setValue(memory, params[0], queueGet(&inputQueue));
        instructionPointer += 2;
        break;
      case 4:
        printf("[OUTPUT] %d\n", getValue(memory, params[0]));
        instructionPointer += 2;
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

  char line[3000];
  fgets(line, 3000, fp);
  fclose(fp);

  int sourceCode[MEMORY] = {0};
  int codeLength = 0;
  
  char *token = 0;
  while ((token = strtok(token ? NULL : line, ","))) {
    sourceCode[codeLength++] = atoi(token);
  }

  struct queue inputQueue = (struct queue) {
    .length = 1
  };
  inputQueue.items[0] = 1;

  runProgram(sourceCode, -1, -1, inputQueue);
}
