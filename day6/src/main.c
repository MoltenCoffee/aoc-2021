#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

#include "../../lib/common.h"

#define DAYS 256

#define NEW_FISH_VALUE 8
#define RESET_FISH_VALUE 6

int main(int argc, char *argv[])
{
  printf("\n");
  if (argc != 2)
  {
    fprintf(stderr, "Exactly one argument expected: path to input\n");
    return EXIT_FAILURE;
  }
  char *file = readFile(argv[1]);

  uint64_t stages[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

  // Insert input data into array
  int charCount = 0;
  for (;;)
  {
    char c = file[charCount];
    if (c == '\0')
      break;
    if (isdigit(c))
    {
      switch (c)
      {
      case '1':
        stages[1]++;
        break;
      case '2':
        stages[2]++;
        break;
      case '3':
        stages[3]++;
        break;
      case '4':
        stages[4]++;
        break;
      case '5':
        stages[5]++;
        break;
      }
    }
    charCount++;
  }

  for (int i = 0; i < DAYS; i++)
  {
    uint64_t newStages[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    // Build new array
    for (int i = 0; i < 9; i++)
    {
      if (i == 0)
      {
        newStages[RESET_FISH_VALUE] += stages[0];
        newStages[NEW_FISH_VALUE] += stages[0];
      }
      else
      {
        newStages[i - 1] += stages[i];
      }
    }

    // update old array
    for (int i = 0; i < 9; i++)
    {
      stages[i] = newStages[i];
    }
  }

  uint64_t totalFish = stages[0] + stages[1] + stages[2] + stages[3] + stages[4] + stages[5] + stages[6] + stages[7] + stages[8];
  printf("Total fish: %ld\n", totalFish);

  return EXIT_SUCCESS;
}
