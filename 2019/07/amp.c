#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>

#define MEMORY 1024
#define MAX_PARAMS 3
#define MAX_QUEUE_LENGTH 8
#define DEBUG_LEVEL 0

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
    return INT_MIN;
  }
  int value = (*q).items[0];
  memcpy((*q).items, &(*q).items[1], sizeof(int) * (MAX_QUEUE_LENGTH - 1));
  (*q).length--;
  return value;
}

struct queue* newQueue(int nInitValues, ...) {
  struct queue *q = malloc(sizeof(struct queue));

  va_list args;
  va_start(args, nInitValues);
  for (int i = 0; i < nInitValues; i++) {
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

struct programState {
  int id;
  int halt;
  int instructionPointer;
  int memory[MEMORY];
};

struct programState* initState(int inititalMemory[MEMORY], int id) {
  struct programState* state = malloc(sizeof(struct programState));

  state->id = id;
  state->halt = 0;
  state->instructionPointer = 0;
  memcpy(state->memory, inititalMemory, sizeof(int) * MEMORY);

  return state;
}

int runProgram(struct programState *state, struct queue* inputQueue, struct queue* outputQueue) {  
  while (!state->halt) {
    int opCode, parameterModes[MAX_PARAMS] = {0}, params[MAX_PARAMS];
    parseInstruction(&opCode, parameterModes, state->memory[state->instructionPointer]);
    getParameters(state->memory, parameterModes, state->instructionPointer, params);

    debugLog(2, "Running instruction %d", opCode);

    int condition, scratch;
    
    switch (opCode) {
      case 1: // ADD: Param 3 = param 1 + param 2
        setValue(state->memory, params[2], getValue(state->memory, params[0]) + getValue(state->memory, params[1]));
        state->instructionPointer = getNextInstructionPointer(state->instructionPointer, 3);
        break;
      case 2: // MUL: Param 3 = param 1 * param 2
        setValue(state->memory, params[2], getValue(state->memory, params[0]) * getValue(state->memory, params[1]));
        state->instructionPointer = getNextInstructionPointer(state->instructionPointer, 3);
        break;
      case 3: // INP: Param 1 = input
        scratch = queueGet(inputQueue);
        if (scratch == INT_MIN) return 0;
        setValue(state->memory, params[0], scratch);
        state->instructionPointer = getNextInstructionPointer(state->instructionPointer, 1);
        break;
      case 4: // OUT: Output param 1
        debugLog(1, "Outputting %d", getValue(state->memory, params[0]));
        queuePush(outputQueue, getValue(state->memory, params[0]));
        state->instructionPointer = getNextInstructionPointer(state->instructionPointer, 1);
        break;
      case 5: // JMPT: jump to param 2 if param 1 != 0
        condition = getValue(state->memory, params[0]);
        state->instructionPointer = condition != 0 ? getValue(state->memory, params[1]) : getNextInstructionPointer(state->instructionPointer, 2);
        break;
      case 6: // JMPF: jump to param 2 if param 1 == 0
        condition = getValue(state->memory, params[0]);
        state->instructionPointer = condition == 0 ? getValue(state->memory, params[1]) : getNextInstructionPointer(state->instructionPointer, 2);
        break;
      case 7: // LESS param 3 = param 1 < param 2 ? 1 : 0
        setValue(state->memory, params[2], getValue(state->memory, params[0]) < getValue(state->memory, params[1]));
        state->instructionPointer = getNextInstructionPointer(state->instructionPointer, 3);
        break;
      case 8: // EQ param 3 = param 1 == param 2
        setValue(state->memory, params[2], getValue(state->memory, params[0]) == getValue(state->memory, params[1]));
        state->instructionPointer = getNextInstructionPointer(state->instructionPointer, 3);
        break;
      case 99:
        state->halt = 1;
        break;
      default:
        printf("Unknown opCode %d\n", opCode);
        exit(EXIT_FAILURE);
    }
  }

  return 1;
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

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int next_permutation(int* arr, int n) {
    int i = n - 2;
    while (i >= 0 && arr[i] >= arr[i + 1]) {
        i--;
    }
    if (i < 0) {
        return 0;
    }
    int j = n - 1;
    while (arr[j] <= arr[i]) {
        j--;
    }
    swap(&arr[i], &arr[j]);
    int k = i + 1;
    j = n - 1;
    while (k < j) {
        swap(&arr[k], &arr[j]);
        k++;
        j--;
    }
    return 1;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("[ERROR] Missing parameter <filename>\n");
    exit(EXIT_FAILURE);
  }

  char *filename = argv[1];
  int *program = readSourceCode(filename);

  int sequence[] = {0, 1, 2, 3, 4};

  int part1 = 0;
  do {
    struct queue* ioQueues[5] = {
      newQueue(2, sequence[0], 0),
      newQueue(1, sequence[1]),
      newQueue(1, sequence[2]),
      newQueue(1, sequence[3]),
      newQueue(1, sequence[4]),
    };
    struct programState* states[5] = {
      initState(program, 0),
      initState(program, 1),
      initState(program, 2),
      initState(program, 3),
      initState(program, 4),
    };

    for (int s = 0; s < 5; s++) {
      runProgram(states[s], ioQueues[s], ioQueues[(s + 1) % 5]);
    }
    int finalThrust = queueGet(ioQueues[0]);
    if (finalThrust > part1) part1 = finalThrust;
  
    for (int i = 0; i < 5; i++) {
      free(ioQueues[i]);
      free(states[i]);
    }
  } while (next_permutation(sequence, 5));

  printf("Part 1: %d\n", part1);

  int sequence2[] = {5, 6, 7, 8, 9};
  int part2 = 0;
  do {
    struct queue* ioQueues[5] = {
      newQueue(2, sequence2[0], 0),
      newQueue(1, sequence2[1]),
      newQueue(1, sequence2[2]),
      newQueue(1, sequence2[3]),
      newQueue(1, sequence2[4]),
    };
    struct programState* states[5] = {
      initState(program, 0),
      initState(program, 1),
      initState(program, 2),
      initState(program, 3),
      initState(program, 4),
    };

    while (!states[4]->halt) {
      for (int s = 0; s < 5; s++) {
        runProgram(states[s], ioQueues[s], ioQueues[(s + 1) % 5]);
      }
    }
    int finalThrust = queueGet(ioQueues[0]);
    if (finalThrust > part2) part2 = finalThrust;
  
    for (int i = 0; i < 5; i++) {
      free(ioQueues[i]);
      free(states[i]);
    }
  } while (next_permutation(sequence2, 5));

  printf("Part 2: %d\n", part2);

  free(program);
}
