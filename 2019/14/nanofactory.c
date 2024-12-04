#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct ingredient {
  char name[8];
  long long quantity;
};

struct product {
  char name[8];
  long long quantity;
  long long ingredientCount;
  struct ingredient ingredients[16];
  long long stock;
};

long long findIndex(struct product products[64], char *name) {
  int i = 0;
  do {
    if (strcmp(products[i].name, name) == 0) return i;
  } while (++i < 64);
  return -1;
}

long long ceilDiv(long long a, long long b) {
  return (a + b - 1) / b;
}

long long run(struct product products[], int productCount, long long fuelCount) {
  long long oreNeeded = 0;
  struct ingredient needs[128];
  int needCount = 1;
  strcpy(needs[0].name, "FUEL");
  needs[0].quantity = fuelCount;

  for (int p = 0; p < productCount; p++) products[p].stock = 0;
  
  while (needCount) {
    struct ingredient newNeeds[128];
    int newNeedCount = 0;
    for (int i = 0; i < needCount; i++) {
      struct ingredient ing = needs[i];
      struct product *p = &products[findIndex(products, ing.name)];;
      
      if (p->stock >= ing.quantity) {
        p->stock -= ing.quantity;
      } else {
        long long remaining = ing.quantity - p->stock;
        p->stock = 0;

        long long reactions = ceilDiv(remaining, p->quantity);
        p->stock = (reactions * p->quantity) - remaining;
        for (int pi = 0; pi < p->ingredientCount; pi++) {
          if (strcmp(p->ingredients[pi].name, "ORE") == 0) {
            oreNeeded += reactions * p->ingredients[pi].quantity;
          } else {
            int present = 0;
            for (int nn = 0; nn < newNeedCount; nn++) {
              if (strcmp(newNeeds[nn].name, p->ingredients[pi].name) == 0) {
                present = 1;
                newNeeds[nn].quantity += reactions * p->ingredients[pi].quantity;
              }
            }
            if (present == 0) {
              newNeeds[newNeedCount].quantity = reactions * p->ingredients[pi].quantity;
              strcpy(newNeeds[newNeedCount].name, p->ingredients[pi].name);
              newNeedCount++;
            }
          }
        }
      }
    }
    memcpy(needs, newNeeds, sizeof(struct ingredient) * 128);
    needCount = newNeedCount;
  }

  return oreNeeded;
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

  struct product products[64];
  products[0].stock = 0;
  products[0].ingredientCount = 0;
  strcpy(products[0].name, "ORE");
  
  int productCount = 1;
  int fuelIndex = 0;

  char line[128];
  while(fgets(line, 128, fp)) {
    products[productCount].ingredientCount = 0;
    products[productCount].stock = 0;

    char *token = strtok(line, "=");
    char *ingredient;
    char *rest = token;
    while((ingredient = strtok_r(rest , ",", &rest))) {
      long long quantity;
      char name[8] = {0};
      sscanf(ingredient, "%lld %s", &quantity, name);
      
      long long count = products[productCount].ingredientCount++;
      products[productCount].ingredients[count].quantity = quantity;
      memcpy(products[productCount].ingredients[count].name, name, sizeof(char) * 8);
    }

    token = strtok(NULL, ">");
    long long quantity;
    char name[8] = {0};
    sscanf(token, "%lld %s", &quantity, name);
    products[productCount].quantity = quantity;
    memcpy(products[productCount].name, name, sizeof(char) * 8);
    if (strcmp(name, "FUEL") == 0) {
      fuelIndex = productCount;
    }

    productCount++;
  }
  fclose(fp);
  
  long long part1 = run(products, productCount, 1);
  printf("Part 1: %lld\n", part1);

  long long estimate = 1000000000000 / part1;

  long long oreNeeded = 0;
  while (oreNeeded < 1000000000000) {
    oreNeeded = run(products, productCount, estimate);
    long long offset = llabs(oreNeeded - 1000000000000) / (oreNeeded / estimate);
    if (offset < 1) offset = 1;
    estimate += offset;
  }
  while (oreNeeded > 1000000000000) {
    estimate--;
    oreNeeded = run(products, productCount, estimate);
  }

  printf("Part 2: %lld\n", estimate);  
}
