#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

#include "../../lib/common.h"

#define INPUT_SIZE 1000

static int compare(const void *a, const void *b)
{
  uint16_t int_a = *((uint16_t *)a);
  uint16_t int_b = *((uint16_t *)b);

  if (int_a == int_b)
    return 0;
  else if (int_a < int_b)
    return -1;
  else
    return 1;
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "Exactly one argument expected: path to input\n");
    return EXIT_FAILURE;
  }
  char *file = readFile(argv[1]);

  uint16_t input[INPUT_SIZE];
  int inputCount = 0;

  int charCount = 0;
  char buffer[5];
  int bufferCount = 0;
  for (;;)
  {
    char c = file[charCount];
    charCount++;

    if (isdigit(c))
    {
      buffer[bufferCount] = c;
      bufferCount++;
    }
    else
    {
      if (buffer[0] == '\0')
        continue;
      buffer[bufferCount] = '\0';

      input[inputCount] = atoi(buffer);
      inputCount++;

      buffer[0] = '\0';
      bufferCount = 0;
      if (c == '\0')
        break;
    }
  }

  // Not the fastest, but it works
  qsort(input, inputCount, sizeof(uint16_t), compare);

  // ======
  // Part 1
  // ======
  uint32_t fuelUsed = 0;
  uint16_t median = input[INPUT_SIZE / 2];
  for (int i = 0; i < inputCount; i++)
  {
    fuelUsed += abs(input[i] - median);
  }

  printf("Part 1: %d units of fuel used.\n", fuelUsed);

  // ======
  // Part 2
  // ======
  uint64_t fuelUsed2 = 0;
  uint64_t temp = 0;
  uint16_t max = input[inputCount - 1];
  for (uint16_t i = 0; i < max; i++)
  {
    temp = 0;
    for (int j = 0; j < inputCount; j++)
    {
      uint64_t delta = abs(i - input[j]);
      temp += delta * (delta + 1) / 2;
    }
    if (i == 0 || temp < fuelUsed2)
      fuelUsed2 = temp;
  }

  printf("Part 2: %ld units of fuel used.\n", fuelUsed2);

  return EXIT_SUCCESS;
}
