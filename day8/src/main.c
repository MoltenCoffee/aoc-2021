#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>

#include "../../lib/common.h"

#define TOTAL_LINES 200

typedef char segment[8];
typedef segment row[14];

typedef int keyRow[10];
typedef int outputRow[4];
typedef char mapRow[7];

static int findIndex(int *array, int length, int value)
{
  for (int i = 0; i < length; i++)
  {
    if (array[i] == value)
      return i;
  }
  return NULL;
}

static int segmentsInCommon(keyRow translation, row currentRow, int currentPos, int b)
{
  int index = findIndex(translation, 10, b);
  int found = 0;

  int currentLength = strlen(currentRow[currentPos]);
  int compareLength = strlen(currentRow[index]);

  for (int k = 0; k < compareLength; k++)
  {
    char c = currentRow[index][k];
    for (int m = 0; m < currentLength; m++)
    {
      if (currentRow[currentPos][m] == c)
      {
        found++;
        break;
      }
    }
  }

  return found;
}

void sortCharsAlphabetically(char *string)
{
  int i, j;
  char temp;
  int length = strlen(string);

  for (i = 0; i < length - 1; i++)
  {
    for (j = i + 1; j < length; j++)
    {
      if (string[i] > string[j])
      {
        temp = string[i];
        string[i] = string[j];
        string[j] = temp;
      }
    }
  }
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "Exactly one argument expected: path to input\n");
    return EXIT_FAILURE;
  }
  char *file = readFile(argv[1]);

  row rows[TOTAL_LINES];
  int rowCount = 0;

  char *fileLine = readLine(file);
  while (fileLine != NULL)
  {
    int charCount = 0, bufferCount = 0, itemCount = 0;
    char prev;

    for (;;)
    {
      char c = fileLine[charCount];
      charCount++;

      if ((c == ' ' && prev != '|') || c == '\0')
      {
        // End string in buffer
        rows[rowCount][itemCount][bufferCount] = '\0';
        sortCharsAlphabetically(rows[rowCount][itemCount]);
        itemCount++;
        bufferCount = 0;
        if (c == '\0')
          break;
      }
      else if (isalpha(c))
      {
        // Add character to buffer
        rows[rowCount][itemCount][bufferCount] = c;
        bufferCount++;
      }
      prev = c;
    }
    rowCount++;
    fileLine = readLine(NULL);
  }
  free(file);

  keyRow translations[TOTAL_LINES];
  outputRow outputs[TOTAL_LINES];

  uint16_t count1478 = 0;
  uint32_t answer = 0;
  for (int i = 0; i < TOTAL_LINES; i++)
  {
    // Find 1, 4, 7, 8
    for (int j = 0; j < 14; j++)
    {
      int length = strlen(rows[i][j]);
      int translated = 0;

      switch (length)
      {
      case 2:
        translated = 1;
        break;
      case 3:
        translated = 7;
        break;
      case 4:
        translated = 4;
        break;
      case 7:
        translated = 8;
        break;
      default:
        translated = 0;
        break;
      }

      if (j > 9)
      {
        outputs[i][j - 10] = translated;
      }
      else
      {
        translations[i][j] = translated;
      }
    }

    for (int j = 0; j < 10; j++)
    {
      int length = strlen(rows[i][j]);
      // Find 9, 6 0
      if (length == 6)
      {
        int inCommon = segmentsInCommon(translations[i], rows[i], j, 4);
        if (inCommon == 4)
        {
          translations[i][j] = 9;
        }
        else
        {
          int inCommon = segmentsInCommon(translations[i], rows[i], j, 7);
          if (inCommon == 2)
          {
            translations[i][j] = 6;
          } // else 0
        }
      }
      // Find 2, 3, 5
      else if (length == 5)
      {
        int inCommon = segmentsInCommon(translations[i], rows[i], j, 1);
        if (inCommon == 2)
        {
          translations[i][j] = 3;
        }
        else
        {
          int inCommon = segmentsInCommon(translations[i], rows[i], j, 4);
          if (inCommon == 3)
          {
            translations[i][j] = 5;
          }
          else
          {
            translations[i][j] = 2;
          }
        }
      }
    }

    // ======
    // Part 1
    // ======

    for (int j = 0; j < 4; j++)
    {
      if (outputs[i][j] != 0)
        count1478++;
    }

    // ======
    // Part 2
    // ======
    char buffer[5];
    int bufferCount = 0;
    for (int j = 0; j < 4; j++)
    {
      if (outputs[i][j] != 0)
      {
        // Convert int to ASCII char ('0' = 48)
        buffer[bufferCount] = (char)(outputs[i][j] + 48);
        bufferCount++;
      }
      else
      {
        for (int k = 0; k < 10; k++)
        {
          if (strcmp(rows[i][k], rows[i][j + 10]) == 0)
          {
            outputs[i][j] = translations[i][k];
            break;
          }
        }
        // Convert int to ASCII char ('0' = 48)
        buffer[bufferCount] = (char)(outputs[i][j] + 48);
        bufferCount++;
      }

      if (j == 3)
      {
        buffer[bufferCount] = '\0';
        answer += atoi(buffer);

        bufferCount = 0;
        buffer[bufferCount] = '\0';
      }
    }
  }

  printf("Part 1: %d\n", count1478);
  printf("Part 2: %ld\n", answer);

  return EXIT_SUCCESS;
}
