
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../lib/common.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "\033[1;31mExactly one argument expected: path to input\n");
    return EXIT_FAILURE;
  }
  char* file = readFile(argv[1]);

  // Count lines and linelength
  int maxLineLength = 0;
  int currLineLength = 0;
  int lineCount = 0;
  int charCount = 0;
  char prev;
  for (;;) {
    char c = file[charCount];
    charCount++;

    if (c == '\0') {
      if (prev != '\n') {
        lineCount++;

        if (currLineLength > maxLineLength) maxLineLength = currLineLength;
        currLineLength = 0;
      }
      break;
    } else if (c == '\n') {
      lineCount++;

      if (currLineLength > maxLineLength) maxLineLength = currLineLength;
      currLineLength = 0;
      continue;
    }
    currLineLength++;
    prev = c;
  }

  // Allocate memory for lines
  char** lines = calloc(lineCount, sizeof(char*));
  if (lines == NULL) fprintf(stderr, "\033[1;31mCould not allocate memory!");
  for (int i = 0; i < lineCount; i++) {
    lines[i] = calloc(maxLineLength + 1, sizeof(char));
    if (lines[i] == NULL)
      fprintf(stderr, "\033[1;31mCould not allocate memory!");
  }

  // Store data into lines
  lineCount = 0;
  int lineCharCount = 0;
  for (int i = 0; i < charCount; i++) {
    char c = file[i];

    if (c == '\n' || c == '\0') {
      lines[lineCount][lineCharCount] = '\0';
      lineCount++;
      lineCharCount = 0;
      if (c == '\0') break;
    } else {
      lines[lineCount][lineCharCount] = c;
      lineCharCount++;
    }
  }
  free(file);

  // Create stack for syntax cheking
  char* charStack = calloc(maxLineLength + 1, sizeof(char));
  if (charStack == NULL)
    fprintf(stderr, "\033[1;31mCould not allocate memory!");
  int stackPos = 0;

  uint32_t errorScore = 0;

  for (int i = 0; i < lineCount; i++) {
    for (int j = 0; j < maxLineLength; j++) {
      char c = lines[i][j];

      if (c == '\0') break;

      bool hadError = false;

      switch (c) {
        case '(':
        case '[':
        case '{':
        case '<':
          charStack[stackPos] = c;
          stackPos++;
          break;
        case ')':
          if (charStack[stackPos - 1] == '(') {
            stackPos--;
          } else {
            hadError = true;
            errorScore += 3;
          }
          break;
        case ']':
          if (charStack[stackPos - 1] == '[') {
            stackPos--;
          } else {
            hadError = true;
            errorScore += 57;
          }
          break;
        case '}':
          if (charStack[stackPos - 1] == '{') {
            stackPos--;
          } else {
            hadError = true;
            errorScore += 1197;
          }
          break;
        case '>':
          if (charStack[stackPos - 1] == '<') {
            stackPos--;
          } else {
            hadError = true;
            errorScore += 25137;
          }
          break;
      }

      if (hadError) break;
    }
    stackPos = 0;
  }

  printf("Part 1: Error score: %d\n", errorScore);

  // Cleanup
  free(charStack);
  for (int i = 0; i < lineCount; i++) {
    free(lines[i]);
  }
  free(lines);
  return EXIT_SUCCESS;
}
