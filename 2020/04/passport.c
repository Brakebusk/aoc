#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct passport {
  char byr[16], iyr[16], eyr[16], hgt[16], hcl[16], ecl[16], pid[16], cid[16];
};

int validate(struct passport p) {
  return p.byr[0] && p.iyr[0] && p.eyr[0] && p.hgt[0] && p.hcl[0] && p.ecl[0] && p.pid[0];
}

int strictValidate(struct passport p) {
  if (strlen(p.byr) != 4 || strtol(p.byr, NULL, 10) < 1920 || strtol(p.byr, NULL, 10) > 2002) return 0;
  if (strlen(p.iyr) != 4 || strtol(p.iyr, NULL, 10) < 2010 || strtol(p.iyr, NULL, 10) > 2020) return 0;
  if (strlen(p.eyr) != 4 || strtol(p.eyr, NULL, 10) < 2020 || strtol(p.eyr, NULL, 10) > 2030) return 0;
  if (strlen(p.hgt) < 3) return 0;

  char suffix[8] = {0};
  int value = 0;
  sscanf(p.hgt, "%d%s", &value, suffix);
  if (strcmp(suffix, "in") == 0) {
    if (value < 59 || value > 76) return 0;
  } else if (strcmp(suffix, "cm") == 0) {
    if (value < 150 || value > 193) return 0;
  } else {
    return 0;
  }

  char color[16] = {0};
  sscanf(p.hcl, "#%s", color);
  if (strlen(color) != 6) return 0;

  if (!(strcmp(p.ecl, "amb") == 0 || 
        strcmp(p.ecl, "blu") == 0 || 
        strcmp(p.ecl, "brn") == 0 || 
        strcmp(p.ecl, "gry") == 0 || 
        strcmp(p.ecl, "grn") == 0 || 
        strcmp(p.ecl, "hzl") == 0 || 
        strcmp(p.ecl, "oth") == 0)) return 0;
  
  if (strlen(p.pid) != 9 || strtol(p.pid, NULL, 10) < 1) return 0;

  return 1;
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
  int part2 = 0;

  char line[128];
  while(fgets(line, 128, fp)) {
    if (strlen(line) == 1) {
      part1 += validate(p);
      part2 += strictValidate(p);
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
  part2 += strictValidate(p);
  
  printf("Part 1: %d\n", part1);
  printf("Part 2: %d\n", part2);
}
