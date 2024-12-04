#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct ingredient {
  char name[8];
  int quantity;
};

struct product {
  char name[8];
  int quantity;
  int ingredientCount;
  struct ingredient ingredients[16];
  int stock;
};

int findIndex(struct product products[64], char *name) {
  int i = 0;
  do {
    if (strcmp(products[i].name, name) == 0) return i;
  } while (++i < 64);
  return -1;
}

int ceilDiv(int a, int b) {
  return (a + b - 1) / b;
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
      int quantity;
      char name[8] = {0};
      sscanf(ingredient, "%d %s", &quantity, name);
      
      int count = products[productCount].ingredientCount++;
      products[productCount].ingredients[count].quantity = quantity;
      memcpy(products[productCount].ingredients[count].name, name, sizeof(char) * 8);
    }

    token = strtok(NULL, ">");
    int quantity;
    char name[8] = {0};
    sscanf(token, "%d %s", &quantity, name);
    products[productCount].quantity = quantity;
    memcpy(products[productCount].name, name, sizeof(char) * 8);
    if (strcmp(name, "FUEL") == 0) {
      fuelIndex = productCount;
    }

    productCount++;
  }
  fclose(fp);
  
  int part1 = 0;

  struct ingredient needs[128];
  int needCount = 1;
  strcpy(needs[0].name, "FUEL");
  needs[0].quantity = 1;
  
  while (needCount) {
    struct ingredient newNeeds[128];
    int newNeedCount = 0;
    for (int i = 0; i < needCount; i++) {
      struct ingredient ing = needs[i];
      struct product *p = &products[findIndex(products, ing.name)];;
      
      if (p->stock >= ing.quantity) {
        p->stock -= ing.quantity;
      } else {
        int remaining = ing.quantity - p->stock;
        p->stock = 0;

        int reactions = ceilDiv(remaining, p->quantity);
        p->stock = (reactions * p->quantity) - remaining;
        for (int pi = 0; pi < p->ingredientCount; pi++) {
          if (strcmp(p->ingredients[pi].name, "ORE") == 0) {
            part1 += reactions * p->ingredients[pi].quantity;
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

  printf("Part 1: %d\n", part1);
}
