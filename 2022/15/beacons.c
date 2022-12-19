#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Sensor {
  int sX, sY, bX, bY;
};

struct Sensors {
  struct Sensor
};

struct Sensor* createSensor(int sX, int sY, int bX, int bY) {
  struct Sensor* sensor = (struct Sensor*) malloc(sizeof(struct Sensor));
  sensor->sX = sX;
  sensor->sY = sY;
  sensor->bX = bX;
  sensor->bY = bY;
  return sensor;
}

int checkLine(struct Sensor* sensors[30], int sensorCount, int line) {
  for (int )
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

  struct Sensor* sensors[30];
  int sensorCount = 0;
  
  char line[100];
  while(fgets(line, 100, fp)) {
    int sX, sY, bX, bY;
    sscanf(line, "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", &sX, &sY, &bX, &bY);
    sensors[sensorCount++] = createSensor(sX, sY, bX, bY);
  }
  
  fclose(fp);
}