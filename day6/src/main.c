#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

#include "../../lib/common.h"

#define INPUT_SIZE 300
#define ARRAY_GROW_FACTOR 2
#define DAYS 80

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

  // Allocate initial array
  int arraySize = INPUT_SIZE * ARRAY_GROW_FACTOR;
  uint8_t *fish = calloc(arraySize, sizeof(uint8_t));
  if (fish == NULL)
    fprintf(stderr, "Could not allocate memory!\n");
  int fishCount = 0;

  // Insert input data into array
  int charCount = 0;
  for (;;)
  {
    char c = file[charCount];
    if (c == '\0')
      break;
    if (isdigit(c))
    {
      char numstr[2] = {c, '\0'};
      fish[fishCount] = (uint8_t)atoi(numstr);
      fishCount++;
    }
    charCount++;
  }

  // Loop over days
  for (int i = 1; i < DAYS + 1; i++)
  {
    int totalNew = 0;
    // Loop over fish
    for (int j = 0; j < fishCount; j++)
    {
      if (fish[j] == 0)
      {
        fish[j] = RESET_FISH_VALUE;
        totalNew++;
        continue;
      }
      fish[j]--;
    }

    // Check if array allows for new fish, grow if needed
    if (fishCount + totalNew >= arraySize)
    {
      // printf("-- Growing array from %d to %d fish\n", arraySize, arraySize * ARRAY_GROW_FACTOR);
      arraySize = arraySize * ARRAY_GROW_FACTOR;
      fish = realloc(fish, arraySize * sizeof(uint8_t));
      if (fish == NULL)
        fprintf(stderr, "Could not grow memory!\n");
    }

    // printf("After %d day(s), %d new fish added\n", i, totalNew);

    // Add new fish if needed
    if (totalNew > 0)
    {
      for (int k = 0; k < totalNew; k++)
      {
        fish[fishCount] = NEW_FISH_VALUE;
        fishCount++;
      }
    }
  }

  printf("Total fish after %d days: %d\n", DAYS, fishCount);

  free(fish);
  return EXIT_SUCCESS;
}
