#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../lib/common.h"

#define INITIAL_ARRAY_SIZE 1020
#define GROW_FACTOR 2

typedef struct Coord {
  uint16_t X;
  uint16_t Y;
} Coord;

static Coord foldCoords(Coord* coords, int coordCount, char foldDir,
                        uint16_t foldLine) {
  Coord max = {0, 0};
  for (int i = 0; i < coordCount; i++) {
    if (foldDir == 'x') {
      // Fold to left
      if (coords[i].X < foldLine) continue;
      uint16_t newX = foldLine - (coords[i].X - foldLine);
      coords[i].X = newX;
      if (newX > max.X) max.X = newX;
    } else if (foldDir == 'y') {
      // Fold to top
      if (coords[i].Y < foldLine) continue;
      uint16_t newY = foldLine - (coords[i].Y - foldLine);
      coords[i].Y = newY;
      if (newY > max.Y) max.Y = newY;
    }
  }

  return max;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "\033[1;31mExactly one argument expected: path to input\n");
    return EXIT_FAILURE;
  }
  char* file = readFile(argv[1]);

  // Count coords and instructions
  int charCount = 0;
  int lineCount = 0;
  char prev;
  char buffer[5];
  int buffCount = 0;

  int maxCoords = INITIAL_ARRAY_SIZE;
  Coord* coords = calloc(maxCoords, sizeof(Coord));
  if (coords == NULL) fprintf(stderr, "\033[1;31mCould not allocate memory!\n");

  Coord max = {0, 0};

  // Parse coords
  for (;;) {
    char c = file[charCount];
    charCount++;

    if (isdigit(c)) {
      buffer[buffCount] = c;
      buffCount++;
    } else if (c == '\n' || c == ',') {
      buffer[buffCount] = '\0';

      if (lineCount + 1 > maxCoords) {
        maxCoords = maxCoords * GROW_FACTOR * sizeof(Coord);
        coords = realloc(coords, maxCoords);
        if (coords == NULL)
          fprintf(stderr, "\033[1;31mCould not grow memory!\n");
      }

      uint16_t parsed = (uint16_t)atoi(buffer);
      if (c == ',') {
        if (parsed > max.X) max.X = parsed;
        coords[lineCount].X = parsed;
      } else {
        if (parsed > max.Y) max.Y = parsed;
        coords[lineCount].Y = parsed;
      }

      buffCount = 0;
      buffer[buffCount] = '\0';
      if (c == '\n') {
        if (prev == '\n') break;
        lineCount++;
      }
    }

    prev = c;
  }

  // Parse instructions
  int lineCharCount = 0;
  char foldDir;
  uint16_t foldLine;
  for (;;) {
    char c = file[charCount];
    charCount++;

    if (c == '\n' || c == '\0') {
      buffer[buffCount] = '\0';
      foldLine = (uint16_t)atoi(buffer);
      buffCount = 0;
      buffer[buffCount] = '\0';
      lineCharCount = 0;

      Coord localMax = foldCoords(coords, lineCount, foldDir, foldLine);
      if (foldDir == 'x') {
        max.X = localMax.X;
      } else {
        max.Y = localMax.Y;
      }

      if (c == '\0') break;
      // continue;
      break;
    }

    if (lineCharCount == 11) {
      foldDir = c;
    } else if (lineCharCount > 12 && isdigit(c)) {
      buffer[buffCount] = c;
      buffCount++;
    }
    lineCharCount++;
  }
  free(file);

  uint8_t** rows = calloc(max.Y + 1, sizeof(uint8_t*));
  if (rows == NULL) fprintf(stderr, "\033[1;31mCould not allocate memory!\n");
  for (int i = 0; i <= max.Y; i++) {
    rows[i] = calloc(max.X + 1, sizeof(uint8_t));
    if (rows[i] == NULL)
      fprintf(stderr, "\033[1;31mCould not allocate memory!\n");
  }

  uint16_t count = 0;
  for (int i = 0; i < lineCount; i++) {
    if (rows[coords[i].Y][coords[i].X] != 1) {
      rows[coords[i].Y][coords[i].X] = 1;
      count++;
    }
  }

  printf("Part 1: dots after 1 fold: %d\n", count);

  // Cleanup
  for (int i = 0; i <= max.Y; i++) {
    free(rows[i]);
  }
  free(rows);
  free(coords);

  return EXIT_SUCCESS;
}
