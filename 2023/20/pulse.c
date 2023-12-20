#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Types:
0 = broadcaster
1 = flip-flop
2 = conjunction
*/

struct module {
  char type;
  int state; // 0 = off, 1 = on
  char name[16];
  char outConnectionNames[16][16];
  int outConnectionCount;
  int outConnections[16];
  int inConnections[16];
  int inConnectionStates[16];
  int inConnectionCount;
};

struct pulse {
  int source;
  int destination;
  int state; // 0 = low, 1 = high;
};

void removeFirst(struct pulse *queue, int *queueLength) {
  for (int i = 0; i < *queueLength-1; i++) memcpy(&queue[i], &queue[i+1], sizeof(struct pulse));
  (*queueLength)--;
}

void addPulse(struct pulse *queue, int *queueLength, struct pulse p) {
  queue[*queueLength].source = p.source;
  queue[*queueLength].destination = p.destination;
  queue[*queueLength].state = p.state;
  (*queueLength)++;
}

void printModule(struct module m) {
  printf("Module '%s' of type %d has %d out-connections and %d in-connections.\nOut-connections: [", m.name, m.type, m.outConnectionCount, m.inConnectionCount);
  for (int o = 0; o < m.outConnectionCount; o++) printf(o == 0 ? "%d" : ", %d", m.outConnections[o]);
  printf("]\nIn-connections: [");
  for (int i = 0; i < m.inConnectionCount; i++) printf(i == 0 ? "%d" : ", %d", m.inConnections[i]);
  printf("]\nState: %d\n\n", m.state);
}

void printPulse(struct module network[64], struct pulse p) {
  if (p.source == -1) {
    printf("button -");
  } else {
    printf("%s -", network[p.source].name);
  }
  if (p.state) {
    printf("high-> ");
  } else {
    printf("low-> ");
  }
  printf("%s\n", network[p.destination].name);
}

int findModuleByName(struct module modules[64], int moduleCount, char *needle) {
  for (int m = 0; m < moduleCount; m++) {
    if (strcmp(modules[m].name, needle) == 0) return m;
  }
  return -1;
}

int findInconnectionIndex(struct module m, int id) {
  for (int c = 0; c < m.inConnectionCount; c++) {
    if (m.inConnections[c] == id) return c;
  }
  printf("Could not find module with index %d\n", id);
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

  struct module network[64] = {0};
  int moduleCount = 0;

  char line[64];
  while(fgets(line, 64, fp)) {
    char *token = strtok(line, " ");
    
    if (strcmp(token, "broadcaster") == 0) {
      network[moduleCount].type = 0; 
    } else if (token[0] == '%') {
      network[moduleCount].type = 1;
    } else if (token[0] == '&') {
      network[moduleCount].type = 2;
    } else {
      printf("Unknown prefix '%c'\n", token[0]);
      exit(EXIT_FAILURE);
    }
    strcpy(network[moduleCount].name, strcmp(token, "broadcaster") == 0 ? token : &token[1]);
    token = strtok(NULL, " ");

    while ((token = strtok(NULL, ", \n"))) {
      strcpy(network[moduleCount].outConnectionNames[network[moduleCount].outConnectionCount++], token);
    }

    moduleCount++;
  }
  fclose(fp);

  for (int m = 0; m < moduleCount; m++) {
    for (int out = 0; out < network[m].outConnectionCount; out++) {
      network[m].outConnections[out] = findModuleByName(network, moduleCount, network[m].outConnectionNames[out]);
      network[network[m].outConnections[out]].inConnections[network[network[m].outConnections[out]].inConnectionCount++] = m;
    }
  }

  int brodcasterIndex = findModuleByName(network, moduleCount, "broadcaster");
  struct pulse queue[128];
  
  long long pulseCounts[2] = {0};
  
  for (int push = 0; push < 1000; push++) {
    int queueLength = 1;
    queue[0] = (struct pulse) {
      .source = -1,
      .destination = brodcasterIndex,
      .state = 0
    };

    while (queueLength) {
      struct pulse p = queue[0];

      pulseCounts[p.state]++;
      
      struct module destination = network[p.destination];
      switch (destination.type) {
        case 0:
          for (int out = 0; out < destination.outConnectionCount; out++) {
            addPulse(queue, &queueLength, (struct pulse) {
              .source = p.destination,
              .destination = destination.outConnections[out],
              .state = p.state
            });
          }
          break;
        case 1:
          if (p.state == 0) {
            network[p.destination].state = !network[p.destination].state;
            for (int out = 0; out < destination.outConnectionCount; out++) {
              addPulse(queue, &queueLength, (struct pulse) {
                .source = p.destination,
                .destination = destination.outConnections[out],
                .state = network[p.destination].state
              });
            }
          }
          break;
        case 2:
          network[p.destination].inConnectionStates[findInconnectionIndex(destination, p.source)] = p.state;
          int allHigh = 1;
          for (int in = 0; in < destination.inConnectionCount; in++) {
            if (!network[p.destination].inConnectionStates[in]) {
              allHigh = 0;
              break;
            }
          }
          for (int out = 0; out < destination.outConnectionCount; out++) {
            addPulse(queue, &queueLength, (struct pulse) {
              .source = p.destination,
              .destination = destination.outConnections[out],
              .state = !allHigh
            });
          }
          break;
        default:
          printf("Unknown type %d\n", network[p.destination].type);
          exit(EXIT_FAILURE);
      }

      removeFirst(queue, &queueLength);
    }
  }
  printf("Part 1: %lld\n", pulseCounts[0] * pulseCounts[1]);
}
