#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../lib/common.h"

#define ROW_COUNT 10
#define COL_COUNT 10

typedef uint8_t Grid[ROW_COUNT][COL_COUNT];

static void propagateFlash(Grid rows, Grid flashed) {
  // Flash all octopuses with 9 or higher
  bool loopAgain = false;
  for (int i = 0; i < ROW_COUNT; i++) {
    for (int j = 0; j < COL_COUNT; j++) {
      if (rows[i][j] > 9 && flashed[i][j] != 1) {
        flashed[i][j] = 1;
        loopAgain = true;
        // And increase all surrounding
        // above
        if (i > 0) rows[i - 1][j]++;
        // below
        if (i < ROW_COUNT - 1) rows[i + 1][j]++;
        // left
        if (j > 0) rows[i][j - 1]++;
        // right
        if (j < COL_COUNT - 1) rows[i][j + 1]++;

        // above left
        if (i > 0 && j > 0) rows[i - 1][j - 1]++;
        // above right
        if (i > 0 && j < COL_COUNT - 1) rows[i - 1][j + 1]++;
        // below left
        if (i < ROW_COUNT - 1 && j > 0) rows[i + 1][j - 1]++;
        // below right
        if (i < ROW_COUNT - 1 && j < COL_COUNT - 1) rows[i + 1][j + 1]++;
      }
    }
  }
  if (loopAgain) propagateFlash(rows, flashed);
}

static uint8_t step(Grid rows) {
  Grid flashed = {0};

  // Increase all by one
  for (int i = 0; i < ROW_COUNT; i++) {
    for (int j = 0; j < COL_COUNT; j++) {
      rows[i][j]++;
    }
  }

  propagateFlash(rows, flashed);

  // Reset all that flashed, count flashed
  uint8_t flashCount = 0;
  for (int i = 0; i < ROW_COUNT; i++) {
    for (int j = 0; j < COL_COUNT; j++) {
      if (flashed[i][j] == 1) {
        rows[i][j] = 0;
        flashCount++;
      }
    }
  }
  return flashCount;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "\033[1;31mExactly one argument expected: path to input\n");
    return EXIT_FAILURE;
  }
  char* file = readFile(argv[1]);

  Grid rows = {0};

  uint8_t charCount = 0;
  uint8_t lineCount = 0;
  uint8_t colCount = 0;
  char prev;
  for (;;) {
    char c = file[charCount];
    charCount++;

    if (isdigit(c)) {
      rows[lineCount][colCount] = (uint8_t)(c - 48);
      colCount++;
    } else if (c == '\n') {
      if (prev == '\n') continue;
      lineCount++;
      colCount = 0;
    } else if (c == '\0') {
      break;
    }
    prev = c;
  }
  free(file);

  // Model steps
  uint16_t flashCount = 0;
  uint16_t allFlashedInStep;
  for (int i = 0;; i++) {
    uint8_t flashesInStep = step(rows);
    if (i < 100) flashCount += flashesInStep;
    if (flashesInStep == 100) {
      allFlashedInStep = i + 1;
      if (i >= 100) break;
    }
  }

  printf("Part 1: Total flashed after 100 steps: %d\n", flashCount);
  printf("Part 2: All flash in step: %d\n", allFlashedInStep);

  return EXIT_SUCCESS;
}
