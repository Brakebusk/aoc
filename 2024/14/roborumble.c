#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEIGHT 103
#define WIDTH 101

const int quadWidth = WIDTH / 2;
const int quadHeight = HEIGHT / 2;

struct robot {
  int r, c, vr, vc;
};

int mod(int a, int b) {
  return (a % b + b) % b;
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

  struct robot robots[512];
  int count = 0;

  char line[32];
  while(fgets(line, 32, fp)) {
    sscanf(line, "p=%d,%d v=%d,%d", &robots[count].c, &robots[count].r, &robots[count].vc, &robots[count].vr);
    count++;
  }
  fclose(fp);

  for (int s = 0; s < 100; s++) {
    for (int r = 0; r < count; r++) {
      robots[r].c = mod(robots[r].c + robots[r].vc, WIDTH);
      robots[r].r = mod(robots[r].r + robots[r].vr, HEIGHT);
    }
  }

  int matrix[HEIGHT][WIDTH];
  memset(matrix, 0, sizeof(int) * HEIGHT * WIDTH);
  for (int i = 0; i < count; i++) matrix[robots[i].r][robots[i].c]++;

  int quads[4] = {0};
  for (int r = 0; r < HEIGHT; r++) {
    for (int c = 0; c < WIDTH; c++) {
      if (r < quadHeight) {
        if (c < quadWidth) {
          quads[0] += matrix[r][c];
        } else if (WIDTH - c <= quadWidth) {
          quads[1] += matrix[r][c];
        }
      } else if (HEIGHT - r <= quadHeight) {
        if (c < quadWidth) {
          quads[2] += matrix[r][c];
        } else if (WIDTH - c <= quadWidth) {
          quads[3] += matrix[r][c];
        }
      }
    }
  }
  printf("Part 1: %d\n", quads[0] * quads[1] * quads[2] * quads[3]);
}
