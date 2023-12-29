#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>

#define MAX_SOURCE_CODE_LENGTH 8192
#define MEMORY 8192
#define MAX_PARAMS 3
#define MAX_QUEUE_LENGTH 1

#define DEBUG_LEVEL 0

#define GRID_SIZE 62

void debugLog(int level, const char *format, ...) {
    if (DEBUG_LEVEL < level) return;
    char *buffer;
    va_list args;
    va_start(args, format);
    vasprintf(&buffer, format, args);
    va_end(args);

    printf("[DEBUG] %s\n", buffer);
}

void debugMemory(long long memory[MEMORY], int from, int to) {
  for (int i = from; i <= to; i++) printf("%lld ", memory[i]);
  printf("\n");
}

struct queue {
  int length;
  long long items[MAX_QUEUE_LENGTH];
};

int queuePush(struct queue* q, long long value) {
  if ((*q).length == MAX_QUEUE_LENGTH) {
    return 0;
  }
  (*q).items[(*q).length++] = value;
  return 1;
}

long long queueGet(struct queue* q) {
  if ((*q).length == 0) {
    return LLONG_MIN;
  }
  long long value = (*q).items[0];
  memcpy((*q).items, &(*q).items[1], sizeof(long long) * (MAX_QUEUE_LENGTH - 1));
  (*q).length--;
  return value;
}
void queueClear(struct queue* q) {
  (*q).length = 0;
}

struct queue* newQueue(long long nInitValues, ...) {
  struct queue *q = malloc(sizeof(struct queue));

  va_list args;
  va_start(args, nInitValues);
  for (int i = 0; i < nInitValues; i++) {
    queuePush(q, va_arg(args, long long));
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

long long getValue(long long memory[MEMORY], int address) {
  validateAddress(address, "getValue");
  debugLog(3, "Getting addr %d (value %lld)", address, memory[address]);
  return memory[address];
}

void setValue(long long *memory, int address, long long value) {
  validateAddress(address, "setValue");
  debugLog(3, "Setting %d to %lld", address, value);
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

void getParameters(long long memory[MEMORY], int parameterModes[MAX_PARAMS], int instructionPointer, int relativeBase, int *dest) {
  for (int p = 0; p < MAX_PARAMS; p++) {
    int memoryLocation = instructionPointer + 1 + p;
    switch (parameterModes[p]) {
      case 0: // Position mode
        dest[p] = getValue(memory, memoryLocation);
        break;
      case 1: // Immediate mode
        dest[p] = memoryLocation;
        break;
      case 2: // Relative mode
        dest[p] = relativeBase + getValue(memory, memoryLocation);
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
  int relativeBase;
  long long memory[MEMORY];
};

struct programState* initState(long long inititalMemory[MEMORY], int id) {
  struct programState* state = malloc(sizeof(struct programState));

  state->id = id;
  state->halt = 0;
  state->instructionPointer = 0;
  state->relativeBase = 0;
  memcpy(state->memory, inititalMemory, sizeof(long long) * MEMORY);

  return state;
}

int runProgram(struct programState *state, struct queue* inputQueue, struct queue* outputQueue) {  
  while (!state->halt) {
    int opCode, parameterModes[MAX_PARAMS] = {0}, params[MAX_PARAMS];
    parseInstruction(&opCode, parameterModes, state->memory[state->instructionPointer]);
    getParameters(state->memory, parameterModes, state->instructionPointer, state->relativeBase, params);

    debugLog(2, "Running instruction %d (modes: %d %d %d)", opCode, parameterModes[0], parameterModes[1], parameterModes[2]);

    long long condition, scratch;
    
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
        if (scratch == LLONG_MIN) return 3;
        setValue(state->memory, params[0], scratch);
        state->instructionPointer = getNextInstructionPointer(state->instructionPointer, 1);
        break;
      case 4: // OUT: Output param 1
        debugLog(1, "Outputting %lld", getValue(state->memory, params[0]));
        if (!queuePush(outputQueue, getValue(state->memory, params[0]))) return 4;
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
      case 9: // CNGBSE
        state->relativeBase += getValue(state->memory, params[0]);
        state->instructionPointer = getNextInstructionPointer(state->instructionPointer, 1);
        debugLog(2, "Relative base is now %d", state->relativeBase);
        break;
      case 99:
        state->halt = 1;
        break;
      default:
        printf("Unknown opCode %d\n", opCode);
        exit(EXIT_FAILURE);
    }
  }

  return 0;
}

long long* readSourceCode(char *filename) {
  FILE *fp = NULL;
  if ((fp = fopen(filename, "r")) == NULL) {
      printf("[ERROR] Failed to open file %s\n", filename);
      exit(EXIT_FAILURE);
  }

  char line[MAX_SOURCE_CODE_LENGTH];
  fgets(line, MAX_SOURCE_CODE_LENGTH, fp);
  fclose(fp);

  long long* memory = malloc(sizeof(long long) * MEMORY);
  memset(memory, 0, sizeof(long long) * MEMORY);
  
  int currentAddress = 0;
  char *token = 0;
  while ((token = strtok(token ? NULL : line, ","))) {
    memory[currentAddress++] = atoll(token);
  }
  
  return memory;
}

void printGrid(char grid[GRID_SIZE][GRID_SIZE]) {
  for (int row = 0; row < GRID_SIZE; row++) {
    for (int col = 0; col < GRID_SIZE; col++) {
      char c = grid[row][col];
      printf("%c", c != 0 ? c : ' ');
    }
    printf("\n");
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("[ERROR] Missing parameter <filename>\n");
    exit(EXIT_FAILURE);
  }

  char *filename = argv[1];
  long long *program = readSourceCode(filename);

  char (*grid)[GRID_SIZE] = malloc(sizeof(char[GRID_SIZE][GRID_SIZE]));
  memset(grid, 0, sizeof(char[GRID_SIZE][GRID_SIZE]));

  struct queue* ioQueues[2] = {
    newQueue(0),
    newQueue(0),
  };

  int gR = 0;
  int gC = 0;

  struct programState* state = initState(program, 0);
  while (!state->halt) {
    int breakPoint = runProgram(state, ioQueues[0], ioQueues[1]);
    if (breakPoint == 4) {
      char value = queueGet(ioQueues[1]);
      if (value == '\n') {
        gR++;
        gC = 0;
      } else {
        grid[gR][gC++] = value;
      }
      if (gR >= GRID_SIZE || gC >= GRID_SIZE) {
        printf("Grid too small (row: %d, col: %d)\n", gR, gC);
        exit(EXIT_FAILURE);
      }
    }
  }

  int part1 = 0;
  for (int row = 1; row < GRID_SIZE-1; row++) {
    for (int col = 1; col < GRID_SIZE-1; col++) {
      if (grid[row][col] == '#' &&
          grid[row+1][col] == '#' &&
          grid[row-1][col] == '#' &&
          grid[row][col+1] == '#' &&
          grid[row][col-1] == '#') part1 += row * col;
    }
  }

  printGrid(grid);
  printf("Part1: %d\n", part1);

  free(state);
  free(ioQueues[0]);
  free(ioQueues[1]);
  free(program);
}
