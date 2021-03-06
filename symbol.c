#include "symbol.h"

#include <stdlib.h>
#include <malloc.h>
#include <string.h>

/* maintain a vector to store symbols */

int vecSize, vecCount;
char** vec;

int vectorFull() {
  if (vecSize == vecCount) {
    return 1;
  }
  else {
    return 0;
  }
}

void extendVector() {
  void* newVec = malloc(sizeof(char*) * vecSize * 2);
  memcpy(newVec, vec, sizeof(char*) * vecSize);
  vecSize *= 2;
  free(vec);
  vec = newVec;
}

Symbol storeSymbol(char* symbol) {
  int len = strlen(symbol) + 1;
  void* store = malloc(sizeof(char) * len);
  memcpy(store, symbol, sizeof(char) * len);

  if (vectorFull()) {
    extendVector();
  }
  
  vec[vecCount] = store;
  vecCount++;
  return vecCount - 1;
}

Symbol createSymbolFromStr(char* str) {
  int i;
  for (i = 0; i < vecCount; i++) {
    if (strcmp(str, vec[i]) == 0) {
      return i;
    }
  }
  return storeSymbol(str);
}

void deleteSymbol(Symbol symbol) {
  return;
}

int symboleq_(Symbol x, Symbol y) {
  if (x == y) {
    return 1;
  }
  else {
    return 0;
  }
}

void printSymbol(FILE* file, Symbol x) {
  fprintf(file, "%s", vec[x]);
}

void installSymbolPackage() {
  vecSize = 1;
  vecCount = 0;
  vec = malloc(sizeof(char*) * 1);
}
