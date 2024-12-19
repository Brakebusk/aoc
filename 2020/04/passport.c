#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct passport {
  char byr[16], iyr[16], eyr[16], hgt[16], hcl[16], ecl[16], pid[16], cid[16];
};

int validate(struct passport p) {
  return p.byr[0] && p.iyr[0] && p.eyr[0] && p.hgt[0] && p.hcl[0] && p.ecl[0] && p.pid[0];
}

void reset(struct passport *p) {
  p->byr[0] = '\0';
  p->iyr[0] = '\0';
  p->eyr[0] = '\0';
  p->hgt[0] = '\0';
  p->hcl[0] = '\0';
  p->ecl[0] = '\0';
  p->pid[0] = '\0';
  p->cid[0] = '\0';
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

  struct passport p;
  reset(&p);

  int part1 = 0;

  char line[128];
  while(fgets(line, 128, fp)) {
    if (strlen(line) == 1) {
      part1 += validate(p);
      reset(&p);
    } else {
      char *token = 0;
      while ((token = strtok(token ? NULL : line, " \n"))) {
        char *value = token;
        char *key = strtok_r(value, ":", &value);

        if (strcmp(key, "byr") == 0) {
          strcpy(p.byr, value);
        } else if (strcmp(key, "iyr") == 0) {
          strcpy(p.iyr, value);
        } else if (strcmp(key, "eyr") == 0) {
          strcpy(p.eyr, value);
        } else if (strcmp(key, "hgt") == 0) {
          strcpy(p.hgt, value);
        } else if (strcmp(key, "hcl") == 0) {
          strcpy(p.hcl, value);
        } else if (strcmp(key, "ecl") == 0) {
          strcpy(p.ecl, value);
        } else if (strcmp(key, "pid") == 0) {
          strcpy(p.pid, value);
        } else if (strcmp(key, "cid") == 0) {
          strcpy(p.cid, value);
        } else {
          printf("[ERROR] Unknown key '%s'\n", key);
          exit(EXIT_FAILURE);
        }
      }
    }
  }
  fclose(fp);
  part1 += validate(p);
  
  printf("Part 1: %d\n", part1);
}
