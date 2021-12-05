#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "../../lib/common.h"

#define TOTAL_LINES 500

typedef struct Line
{
  int x1;
  int y1;
  int x2;
  int y2;
} Line;

int main(int argc, char *argv[])
{
  printf("\n");
  if (argc != 2)
  {
    fprintf(stderr, "Exactly one argument expected: path to input\n");
    return EXIT_FAILURE;
  }
  char *file = readFile(argv[1]);

  // Read lines
  Line lines[TOTAL_LINES];
  int lineCount = 0;
  int maxX = 0;
  int maxY = 0;

  char *fileLine = readLine(file);
  while (fileLine != NULL)
  {
    if (lineCount == TOTAL_LINES)
      break;
    char buffer[4];
    int bufferCount = 0;
    int charCount = 0;

    int coords[4];
    int coordCount = 0;

    // Loop over characters in line, store coords, find max coords
    for (;;)
    {
      char c = fileLine[charCount];
      charCount++;
      if (isdigit(c))
      {
        buffer[bufferCount] = c;
        bufferCount++;
      }
      else if (c == ',' || c == ' ' || c == '\n' || c == '\0')
      {
        if (buffer[0] == '\0')
          continue;

        buffer[bufferCount] = '\0';
        bufferCount = 0;

        int coord = atoi(buffer);

        if ((coordCount == 0 || coordCount == 2) && coord > maxX)
        {
          maxX = coord;
        }
        else if ((coordCount == 1 || coordCount == 3) && coord > maxY)
        {
          maxY = coord;
        }

        coords[coordCount] = coord;
        coordCount++;

        buffer[0] = '\0';

        if (c == '\n' || c == '\0')
        {
          coordCount = 0;
          break;
        }
      }
    }

    lines[lineCount].x1 = coords[0];
    lines[lineCount].y1 = coords[1];
    lines[lineCount].x2 = coords[2];
    lines[lineCount].y2 = coords[3];

    lineCount++;

    fileLine = readLine(NULL);
  }

  // Allocate memory for grid
  int **grid = calloc(maxX + 1, sizeof(int *));
  if (grid == NULL)
    fprintf(stderr, "Could not allocate memory!\n");
  for (int i = 0; i < maxX + 1; i++)
  {
    grid[i] = calloc(maxY + 1, sizeof(int));
    if (grid[i] == NULL)
      fprintf(stderr, "Could not allocate memory!\n");
  }

  // Fill grid with horizontal and vertical lines
  for (int i = 0; i < lineCount; i++)
  {
    int x1 = lines[i].x1, x2 = lines[i].x2, y1 = lines[i].y1, y2 = lines[i].y2;
    int low, high, current;

    if (x1 == x2)
    {
      // Handle vertical
      if (y1 > y2)
      {
        high = y1;
        low = y2;
      }
      else
      {
        high = y2;
        low = y1;
      }
      current = low;
      while (current <= high)
      {
        grid[x1][current]++;
        current++;
      }
    }
    else if (y1 == y2)
    {
      // Handle horizontal
      if (x1 > x2)
      {
        high = x1;
        low = x2;
      }
      else
      {
        high = x2;
        low = x1;
      }
      current = low;
      while (current <= high)
      {
        grid[current][y1]++;
        current++;
      }
    }
    else
    {
      // Ignore diagonal
      continue;
    }
  }

  int dangerCount = 0;
  for (int x = 0; x < maxX + 1; x++)
  {
    for (int y = 0; y < maxY + 1; y++)
    {
      if (grid[x][y] >= 2)
      {
        dangerCount++;
      }
    }
  }

  printf("%d\n", dangerCount);

  // cleanup
  for (int i = 0; i < maxX + 1; i++)
  {
    free(grid[i]);
  }
  free(grid);

  return EXIT_SUCCESS;
}
