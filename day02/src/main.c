#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define INPUT_SIZE 1000

typedef enum
{
  UP,
  DOWN,
  FORWARD
} Direction;

typedef struct
{
  Direction direction;
  int amount;
} Instruction;

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

  Instruction *input = (Instruction *)calloc(INPUT_SIZE, sizeof(Instruction));
  int inputCount = 0;

  for (;;)
  {
    if (feof(file))
      break;
    char c = fgetc(file);
    if (isspace(c))
      continue;

    Direction currDir;

    switch (c)
    {
    case 'u':
      currDir = UP;
      break;
    case 'd':
      currDir = DOWN;
      break;
    case 'f':
      currDir = FORWARD;
      break;
    }

    for (;;)
    {
      if (feof(file))
        break;
      c = fgetc(file);
      if (isspace(c))
        continue;

      if (isdigit(c))
      {
        char numString[2] = {c, '\0'};
        int amount = atoi(numString);

        Instruction newInstr;
        newInstr.direction = currDir;
        newInstr.amount = amount;
        input[inputCount] = newInstr;
        inputCount++;
        break;
      }
    }
  }

  // ++++++++
  // Part one
  // ++++++++

  int horz1 = 0;
  int depth1 = 0;

  for (int i = 0; i < inputCount; i++)
  {
    switch (input[i].direction)
    {
    case UP:
      depth1 -= input[i].amount;
      break;
    case DOWN:
      depth1 += input[i].amount;
      break;
    case FORWARD:
      horz1 += input[i].amount;
      break;
    }
  }

  printf("Part 1:\n");
  printf("Horizontal distance: %d\n", horz1);
  printf("Depth: %d\n", depth1);
  printf("Multiplied: %d\n", horz1 * depth1);

  // ++++++++
  // Part two
  // ++++++++

  int aim = 0;
  int horz2 = 0;
  int depth2 = 0;

  for (int i = 0; i < inputCount; i++)
  {
    switch (input[i].direction)
    {
    case UP:
      aim -= input[i].amount;
      break;
    case DOWN:
      aim += input[i].amount;
      break;
    case FORWARD:
      horz2 += input[i].amount;
      depth2 += aim * input[i].amount;
      break;
    }
  }

  printf("\n\nPart 2:\n");
  printf("Horizontal distance: %d\n", horz2);
  printf("Depth: %d\n", depth2);
  printf("Multiplied: %d\n", horz2 * depth2);

  free(input);
  return EXIT_SUCCESS;
}
