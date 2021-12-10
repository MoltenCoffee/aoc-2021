#include <stdio.h>
#include <stdlib.h>

#include "../../lib/common.h"

#define LINES 1000

static char getMostPrevalentChar(char **array, int lines, int pos)
{
  int oneCount = 0;
  int totalCount = 0;

  for (int i = 0; i < lines; i++)
  {
    if (array[i][0] == 'n')
      continue;
    totalCount++;
    if (array[i][pos] == '1')
      oneCount++;
  }

  // Cast to float is critical! Otherwise result will be int as well, causing truncation
  if (oneCount == totalCount / (float)2)
    return '1';
  return oneCount > (totalCount / (float)2) ? '1' : '0';
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "Exactly one argument expected: path to input\n");
    return EXIT_FAILURE;
  }
  char *file = readFile(argv[1]);

  char *line = readLine(file);
  typedef char storedLine[13];
  char **lines = calloc(LINES, sizeof(storedLine));
  if (lines == NULL)
  {
    fprintf(stderr, "Couldn't allocate memory!\n");
    exit(EXIT_FAILURE);
  }

  // ++++++++
  // Part one
  // ++++++++

  int count[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int totalLines = 0;

  while (line != NULL)
  {
    for (int i = 0; i < 12; i++)
    {
      if (line[i] == '1')
        count[i]++;
    }
    lines[totalLines] = line;
    totalLines++;
    line = readLine(NULL);
  }

  int threshold = totalLines / 2;
  char mostPrevalent[13];
  char leastPrevalent[13];

  for (int i = 0; i < 12; i++)
  {
    mostPrevalent[i] = (count[i] > threshold) ? '1' : '0';
    leastPrevalent[i] = (count[i] > threshold) ? '0' : '1';
  }
  mostPrevalent[12] = '\0';

  char *ptr;
  printf("Answer part 1: %ld\n", strtol(mostPrevalent, &ptr, 2) * strtol(leastPrevalent, &ptr, 2));

  // ++++++++
  // Part two
  // ++++++++

  char **oxy = calloc(totalLines, sizeof(char *));
  char **co2 = calloc(totalLines, sizeof(char *));
  if (oxy == NULL || co2 == NULL)
  {
    fprintf(stderr, "Couldn't allocate memory!\n");
    exit(EXIT_FAILURE);
  }

  int oxyCount = 0;
  int co2Count = 0;

  // Move relevant lines to correct array and count them
  for (int i = 0; i < totalLines; i++)
  {
    if (lines[i][0] == mostPrevalent[0])
    {
      oxy[oxyCount] = lines[i];
      oxyCount++;
    }
    else
    {
      co2[co2Count] = lines[i];
      co2Count++;
    }
  }

  int oxyRemaining = oxyCount;
  int co2Remaining = co2Count;

  // Loop over oxygen
  for (int i = 1; i < 12; i++)
  {
    if (oxyRemaining == 1)
      break;

    char mostPrevalentChar = getMostPrevalentChar(oxy, oxyCount, i);

    for (int j = 0; j < oxyCount; j++)
    {
      if (oxyRemaining == 1)
        break;
      if (oxy[j][0] == 'n')
        continue;

      if (oxy[j][i] != mostPrevalentChar)
      {
        oxy[j] = "nnnnnnnnnnnn";
        oxyRemaining--;
      }
    }
  }

  // Loop over CO2
  for (int i = 1; i < 12; i++)
  {
    if (co2Remaining == 1)
      break;
    char mostPrevalentChar = getMostPrevalentChar(co2, co2Count, i);

    for (int j = 0; j < co2Count; j++)
    {
      if (co2Remaining == 1)
        break;
      if (co2[j][0] == 'n')
        continue;

      if (co2[j][i] == mostPrevalentChar)
      {
        co2[j] = "nnnnnnnnnnnn";
        co2Remaining--;
      }
    }
  }

  long oxyRating;
  long co2Rating;

  // Find the remaining values
  for (int i = 0; i < oxyCount; i++)
  {
    if (oxy[i][0] == 'n')
      continue;
    oxyRating = strtol(oxy[i], &ptr, 2);
    break;
  }
  for (int i = 0; i < co2Count; i++)
  {
    if (co2[i][0] == 'n')
      continue;
    co2Rating = strtol(co2[i], &ptr, 2);
    break;
  }

  printf("Answer part 2: %ld\n", oxyRating * co2Rating);

  free(oxy);
  free(co2);
  free(lines);
  return EXIT_SUCCESS;
}
