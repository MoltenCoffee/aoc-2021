#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../lib/common.h"

typedef struct Point {
  int value;
  bool visited;
} Point;

static bool isLowPoint(Point** rows, int r, int c, int maxRow, int maxCol) {
  int current = rows[r][c].value;
  if (r != 0 && current >= rows[r - 1][c].value) return false;
  if (c != 0 && current >= rows[r][c - 1].value) return false;
  if (r != maxRow && current >= rows[r + 1][c].value) return false;
  if (c != maxCol && current >= rows[r][c + 1].value) return false;

  return true;
}

// This cannot handle multiple low points in same basin. Problem?
static int findBasinSize(Point** rows, int r, int c, int maxRow, int maxCol) {
  int size = 0;

  if (!rows[r][c].visited) {
    rows[r][c].visited = true;
    size++;
  }

  if (r != 0 && rows[r - 1][c].value < 9 && !rows[r - 1][c].visited) {
    size += findBasinSize(rows, r - 1, c, maxRow, maxCol);
  }
  if (c != 0 && rows[r][c - 1].value < 9 && !rows[r][c - 1].visited) {
    size += findBasinSize(rows, r, c - 1, maxRow, maxCol);
  }
  if (r != maxRow && rows[r + 1][c].value < 9 && !rows[r + 1][c].visited) {
    size += findBasinSize(rows, r + 1, c, maxRow, maxCol);
  }
  if (c != maxCol && rows[r][c + 1].value < 9 && !rows[r][c + 1].visited) {
    size += findBasinSize(rows, r, c + 1, maxRow, maxCol);
  }

  return size;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "\033[1;31mExactly one argument expected: path to input\n");
    return EXIT_FAILURE;
  }
  char* file = readFile(argv[1]);

  int rowCount = 0;
  int columnCount = 0;

  // Count columns and rows
  int charCount = 0;
  char prev;
  for (;;) {
    char c = file[charCount];

    if (c == '\0') {
      if (prev == '\n') {
        printf("yes");
        rowCount--;
      } else if (isdigit(prev)) {
        rowCount++;
      }
      break;
    } else if (c == '\n') {
      rowCount++;
    } else if (rowCount == 0 && isdigit(c)) {
      columnCount++;
    }

    prev = c;
    charCount++;
  }

  // Allocate memory
  Point** rows = calloc(sizeof(Point**), rowCount);
  if (rows == NULL) fprintf(stderr, "\033[1;31mCould not allocate memory!\n");
  for (int i = 0; i < rowCount; i++) {
    rows[i] = calloc(sizeof(Point), columnCount);
    if (rows[i] == NULL)
      if (rows == NULL)
        fprintf(stderr, "\033[1;31mCould not allocate memory!\n");
  }

  // Add data to memory
  int matrixRow = 0;
  int matrixColumn = 0;
  for (int i = 0; i < charCount; i++) {
    char c = file[i];
    if (c == '\n') {
      matrixRow++;
      matrixColumn = 0;
      continue;
    } else if (isdigit(c)) {
      rows[matrixRow][matrixColumn].value = (int)(c - 48);
      rows[matrixRow][matrixColumn].visited = false;
      matrixColumn++;
    }
  }
  free(file);

  int maxRow = rowCount - 1;
  int maxCol = columnCount - 1;

  int totalRisk = 0;
  int largestBasins[3] = {0, 0, 0};
  int smallestBigBasin = 0;
  for (int i = 0; i < rowCount; i++) {
    for (int j = 0; j < columnCount; j++) {
      if (isLowPoint(rows, i, j, maxRow, maxCol)) {
        totalRisk += (1 + rows[i][j].value);

        int basinSize = findBasinSize(rows, i, j, maxRow, maxCol);
        if (basinSize > largestBasins[smallestBigBasin]) {
          largestBasins[smallestBigBasin] = basinSize;
          for (int k = 0; k < 3; k++) {
            if (largestBasins[k] < largestBasins[smallestBigBasin]) {
              smallestBigBasin = k;
            }
          }
        }
      }
    }
  }

  uint32_t answer2 = 1;
  for (int i = 0; i < 3; i++) {
    answer2 *= largestBasins[i];
  }

  printf("Part 1: total risk level of lowpoints is: %d\n", totalRisk);
  printf("Part 2: multiplied basin sizes is: %d\n", answer2);

  // Cleanup
  for (int i = 0; i < rowCount; i++) {
    free(rows[i]);
  }
  free(rows);
  return EXIT_SUCCESS;
}
