#include "number.h"

#include <stdlib.h>

Number createNumber(char* str) {
  return atoll(str);
}

void deleteNumber(Number number) {
  return;
}

int numbereq_(Number x, Number y) {
  if (x == y) {
    return 1;
  }
  else {
    return 0;
  }
}

Number numberadd(Number x, Number y) {
  return x + y;
}

void installNumberPackage() {
  return;
}