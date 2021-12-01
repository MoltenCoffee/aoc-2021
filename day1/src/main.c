#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#define ARRAY_SIZE 2048

static inline void emptyBuffer(char *buffer)
{
  buffer[0] = '\0';
}

static inline void addValue(char *buffer, long *values, int i)
{
  values[i] = atol(buffer);
}

static void readValues(FILE *file, long *values)
{
  char buffer[10];
  int curr = 0;
  int total = 0;

  for (;;)
  {
    if (feof(file))
      return;
    char c = fgetc(file);
    if (isdigit(c))
    {
      buffer[curr] = c;
      curr++;
    }
    else if (c == '\n')
    {
      buffer[curr] = '\0';
      addValue(buffer, values, total);
      total++;
      emptyBuffer(buffer);
      curr = 0;
    }
  }
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "Only one argument expected: path to input");
    return EXIT_FAILURE;
  }

  FILE *file = fopen(argv[1], "r");
  if (file == NULL)
  {
    fprintf(stderr, "File %s could not be opened", argv[1]);
    return EXIT_FAILURE;
  }

  long *values = (long *)calloc(ARRAY_SIZE, sizeof(long));
  readValues(file, values);
  fclose(file);

  // ++++++++
  // Part one
  // ++++++++

  int increasedValuesCount = 0;
  for (int i = 1; i < ARRAY_SIZE; i++)
  {
    if (values[i] > values[i - 1])
    {
      increasedValuesCount++;
    }
  }

  printf("Increased values compared to last value: %d\n", increasedValuesCount);

  // ++++++++
  // Part two
  // ++++++++

  int increasedWindowSumCount = 0;
  long prevSum = 0;
  // Not terribly efficient, but it works
  for (int i = 2; i < ARRAY_SIZE; i++)
  {
    long newSum = values[i] + values[i-1] + values[i-2];
    if (i > 2 && newSum > prevSum)
    {
      increasedWindowSumCount++;
    }
    prevSum = newSum;
  }

  printf("Increased window sum compared to last window: %d\n", increasedWindowSumCount);

  // Cleanup
  free(values);
  return EXIT_SUCCESS;
}
