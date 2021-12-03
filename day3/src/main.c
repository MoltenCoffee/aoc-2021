#include <stdio.h>
#include <stdlib.h>

#include "../../lib/common.h"

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "Exactly one argument expected: path to input");
    return EXIT_FAILURE;
  }
  char *file = readFile(argv[1]);

  char *line = readLine(file);

  int count[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int totalLines = 0;

  while (line != NULL)
  {
    for (int i = 0; i < 12; i++)
    {
      if (line[i] == '1')
        count[i]++;
    }
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

  return EXIT_SUCCESS;
}
