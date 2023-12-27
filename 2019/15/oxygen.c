#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>
#include <time.h>

#define MAX_SOURCE_CODE_LENGTH 8192
#define MEMORY 4096
#define MAX_PARAMS 3
#define MAX_QUEUE_LENGTH 3
#define GRID_SIZE 50

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

void printGrid(char grid[GRID_SIZE][GRID_SIZE], int posRow, int posCol) {
  printf("Exploration:\n");
  for (int col = 0; col < GRID_SIZE; col++) {
    printf("-");
  };
  printf("\n");
  for (int row = 0; row < GRID_SIZE; row++) {
    for (int col = 0; col < GRID_SIZE; col++) {
      if (col == 0 || col == GRID_SIZE-1) printf("|");
      if (row == posRow && col == posCol) {
        printf("D");
      } else if (row == GRID_SIZE / 2 && col == GRID_SIZE / 2) {
        printf("S");
      } else {
        switch (grid[row][col]) {
          case 0:
            printf(" ");
            break;
          case 1:
            printf("#");
            break;
          case 2:
            printf("O");
            break;
          case 3:
            printf(".");
            break;
          default:
            printf("Unknown tile: '%d'\n", grid[row][col]);
            exit(EXIT_FAILURE);
        }
      }
    }
    printf("\n");
  }
  for (int col = 0; col < GRID_SIZE; col++) {
    printf("-");
  };
  printf("\n");
}

int randomDirection() {
    static int initialized = 0;
    if (!initialized) {
        srand(time(NULL));
        initialized = 1;
    }
    return rand() % 4;
}

int anyEmpty(char grid[GRID_SIZE][GRID_SIZE]) {
  for (int row = 0; row < GRID_SIZE; row++) {
    for (int col = 0; col < GRID_SIZE; col++) {
      if (grid[row][col] == 3) return 1;
    }
  }
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("[ERROR] Missing parameter <filename>\n");
    exit(EXIT_FAILURE);
  }

  char *filename = argv[1];
  long long *program = readSourceCode(filename);

  char grid[GRID_SIZE][GRID_SIZE] = {0};
  int row = GRID_SIZE/2, col = GRID_SIZE/2;
  grid[row][col] = 3;

  struct queue* ioQueues[2] = {
    newQueue(0),
    newQueue(0),
  };
  int direction = 0;

  int roundsSinceLastUnlock = 0;

  struct programState* state = initState(program, 0);
  while (!state->halt && roundsSinceLastUnlock < 10000000) {
    roundsSinceLastUnlock++;
    if (row == 0 || col == 0 || row == GRID_SIZE-1 || col == GRID_SIZE-1) {
      state->halt = 1;
      printGrid(grid, row, col);
      break;
    }
    int breakPoint = runProgram(state, ioQueues[0], ioQueues[1]);
    if (ioQueues[1]->length) {
      int prevAttempt = queueGet(ioQueues[1]);
      if (prevAttempt == 2) { // Found part
        switch (direction+1) {
          case 1:
            row--;
            break;
          case 2:
            row++;
            break;
          case 3:
            col--;
            break;
          case 4:
            col++;
            break;
          default:
            printf("Unknown direction %d\n", direction+1);
            exit(EXIT_FAILURE);
        }
        grid[row][col] = 2;
      } else if (prevAttempt == 1) { // Empty
        switch (direction+1) {
          case 1:
            row--;
            break;
          case 2:
            row++;
            break;
          case 3:
            col--;
            break;
          case 4:
            col++;
            break;
          default:
            printf("Unknown direction %d\n", direction+1);
            exit(EXIT_FAILURE);
        }
        if (grid[row][col] == 0) roundsSinceLastUnlock = 0;
        grid[row][col] = 3;
      } else { // Wall
        switch (direction + 1) {
          case 1:
            grid[row-1][col] = 1;
            break;
          case 2:
            grid[row+1][col] = 1;
            break;
          case 3:
            grid[row][col-1] = 1;
            break;
          case 4:
            grid[row][col+1] = 1;
            break;
          default:
            printf("Unknown direction %d\n", direction);
            exit(EXIT_FAILURE);
        }
      }
      if (grid[row-1][col] == 0) {
        direction = 0;
      } else if (grid[row+1][col] == 0) {
        direction = 1;
      } else if (grid[row][col-1] == 0) {
        direction = 2;
      } else if (grid[row][col+1] == 0) {
        direction = 3;
      } else {
        direction = randomDirection();
      }
    }
    queuePush(ioQueues[0], direction+1);
  } 
  for (int row = 0; row < GRID_SIZE; row++) {
    for (int col = 0; col < GRID_SIZE; col++) {
      if (grid[row][col] == 0) grid[row][col] = 1;
    }
  }
  printGrid(grid, row, col);

  free(state);
  free(ioQueues[0]);
  free(ioQueues[1]);
  free(program);

  int minutes = 0;
  while (anyEmpty(grid)) {
    minutes++;
    for (int row = 0; row < GRID_SIZE; row++) {
      for (int col = 0; col < GRID_SIZE; col++) {
        if (grid[row][col] == 2) {
          if (grid[row][col+1] == 3) grid[row][col+1] = 4;
          if (grid[row][col-1] == 3) grid[row][col-1] = 4;
          if (grid[row+1][col] == 3) grid[row+1][col] = 4;
          if (grid[row-1][col] == 3) grid[row-1][col] = 4;
        }
      }
    }
    for (int row = 0; row < GRID_SIZE; row++) {
      for (int col = 0; col < GRID_SIZE; col++) {
        if (grid[row][col] == 4) grid[row][col] = 2;
      }
    }
  }

  printGrid(grid, 0, 0);
  printf("Part 2: %d\n", minutes);
}
