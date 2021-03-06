#include "parser.h"
#include "exception.h"

#include <string.h>

#define maxBufferSize 1024
char buffer[maxBufferSize], token[maxBufferSize];
int currentPos, bufferSize;
FILE* inputFile;
Atom* symbolQuote;

int isSpace(char x) {
  if (x == ' ' || x == '\n' || x == '\t' || x == '\r') {
    return 1;
  }
  else {
    return 0;
  }
}

int isReservedChar(char x) {
  if (x == '(' || x == ')') {
    return 1;
  }
  else {
    return 0;
  }
}

int isNormalChar(char x) {
  if (!isSpace(x) && !isReservedChar(x)) {
    return 1;
  }
  else {
    return 0;
  }
}

void scanBuffer() {
  while (1) {
    while (currentPos < bufferSize && isSpace(buffer[currentPos])) {
      currentPos++;
    }
    if (currentPos < bufferSize) {
      return;
    }
    else {
      if (fscanf(inputFile, "%s", buffer) != 1) {
        exception("scan buffer", "can not read anything from file");
        return;
      }
      currentPos = 0;
      bufferSize = strlen(buffer);
    }
  }
}

void nextToken() {
  scanBuffer();

  int lastPos = currentPos;
  char c = buffer[currentPos];
  if (c == '(' || c == ')' || c == '\'') {
    currentPos++;
  }
  else {
    while (currentPos < bufferSize && isNormalChar(buffer[currentPos])) {
      currentPos++;
    }
  }
  
  int len = currentPos - lastPos;
  memcpy(token, buffer + lastPos, sizeof(char) * len);
  token[len] = '\0';
}

int isNumber(char* s) {
  if (*s == '-') {
    s++;
    if (*s == '\0') {
      return 0;
    }
  }
  
  for (; *s; s++) {
    if (*s > '9' || *s < '0') {
      return 0;
    }
  }
  return 1;
}

Atom* parse();

Atom* parseSequence() {
  nextToken();
  if (*token == ')') {
    return nil;
  }
  else {
    Atom* result = parse();
    Atom* subsequence = parseSequence();
    return cons(result, subsequence);
  }
}

Atom* parse() {
  if (*token == '(') {
    return parseSequence();
  }
  else if (*token == ')') {
    exception("parse", "unexpected ')'");
    return nil;
  }
  else if (*token == '\'') {
    nextToken();
    return cons(symbolQuote, cons(parse(), nil));
  }
  else {
    if (isNumber(token)) {
      return createNumber(createNumberFromStr(token));
    }
    else {
      return createSymbol(createSymbolFromStr(token));
    }
  }
}

Atom* parseFromFile(FILE* file) {
  if (file == stdin) {
    fprintf(stderr, "> ");
  }
  symbolQuote = createReservedSymbol("quote");

  inputFile = file;
  currentPos = 0;
  bufferSize = 0;
  nextToken();
  Atom* result = parse();
  if (file == stdin) {
    fprintf(stderr, "< "); // indicate that parsing is finished.
  }
  return result;
}
