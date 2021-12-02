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

  int horz = 0;
  int depth = 0;

  for (int i = 0; i < inputCount; i++)
  {
    switch (input[i].direction)
    {
    case UP:
      depth -= input[i].amount;
      break;
    case DOWN:
      depth += input[i].amount;
      break;
    case FORWARD:
      horz += input[i].amount;
      break;
    }
  }

  printf("Horizontal distance: %d\n", horz);
  printf("Depth: %d\n", depth);
  printf("Multiplied: %d\n", horz * depth);

  free(input);
  return EXIT_SUCCESS;
}
