#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "../../lib/common.h"

#define LIST_SIZE 100
#define BOARD_ROWS 5
#define BOARD_COLS 5

typedef struct Field
{
  int val;
  bool checked;
} Field;

typedef Field Row[BOARD_COLS];
typedef Row Board[BOARD_ROWS];

static int calcRemaining(Board board)
{
  int sum = 0;
  for (int i = 0; i < BOARD_ROWS; i++)
  {
    for (int j = 0; j < BOARD_COLS; j++)
    {
      if (!board[i][j].checked)
      {
        sum += board[i][j].val;
      }
    }
  }

  return sum;
}

static bool crossFields(Board board, int draw)
{
  // 5 rows, 5 cols
  int checkCounts[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  for (int i = 0; i < BOARD_ROWS; i++)
  {
    for (int j = 0; j < BOARD_COLS; j++)
    {
      if (board[i][j].checked)
      {
        checkCounts[i]++;
        checkCounts[j + BOARD_ROWS]++;
        continue;
      }
      if (board[i][j].val == draw)
      {
        board[i][j].checked = true;
        checkCounts[i]++;
        checkCounts[j + BOARD_ROWS]++;
      }
    }
  }

  for (int i = 0; i < 10; i++)
  {
    if (checkCounts[i] == 5)
    {
      return true;
    }
  }

  return false;
}

static void printBoard(Board board)
{
  for (int i = 0; i < BOARD_ROWS; i++)
  {
    printf("%d, %d, %d, %d, %d\n", board[i][0].checked, board[i][1].checked, board[i][2].checked, board[i][3].checked, board[i][4].checked);
  }
  printf("\n");
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "Exactly one argument expected: path to input\n");
    return EXIT_FAILURE;
  }
  char *file = readFile(argv[1]);

  // Loop utils
  char buffer[3];
  int bufferCount = 0;

  // Read draw numbers from input
  int draw[LIST_SIZE];
  int drawCount = 0;
  int charCount = 0;
  for (;;)
  {
    char c = file[charCount];
    charCount++;

    if (isdigit(c))
    {
      buffer[bufferCount] = c;
      bufferCount++;
    }
    else if (c == ',' || c == '\n')
    {
      buffer[bufferCount] = '\0';
      draw[drawCount] = atoi(buffer);
      buffer[0] = '\0';
      bufferCount = 0;
      drawCount++;
    }

    if (c == '\n')
      break;
  }

  // Read boards from input
  Board boards[LIST_SIZE];
  int boardCount = 0;
  int currentRow = 0;
  int currentCol = 0;
  for (;;)
  {
    char c = file[charCount];
    charCount++;

    if (isdigit(c))
    {
      buffer[bufferCount] = c;
      bufferCount++;
    }
    else if (c == ' ' || c == '\n' || ((c == '\0' && buffer[0] != '\0')))
    {
      if (buffer[0] == '\0')
        continue;
      buffer[bufferCount] = '\0';
      boards[boardCount][currentRow][currentCol].val = atoi(buffer);
      boards[boardCount][currentRow][currentCol].checked = false;
      buffer[0] = '\0';
      bufferCount = 0;

      if (c == '\0')
      {
        boardCount++;
        break;
      }

      if (currentCol == BOARD_COLS - 1 && currentRow == BOARD_ROWS - 1)
      {
        boardCount++;
        currentCol = 0;
        currentRow = 0;
      }
      else if (currentCol >= BOARD_COLS - 1)
      {
        currentRow++;
        currentCol = 0;
      }
      else
      {
        currentCol++;
      }
    }
  }

  // Loop over draw numbers, loop over boards inside to mark
  int ammountWon = 0;
  bool wonArray[LIST_SIZE];
  for (int i = 0; i < drawCount; i++)
  {
    for (int j = 0; j < boardCount; j++)
    {
      if (wonArray[j] == true)
        continue;
      bool won = crossFields(boards[j], draw[i]);
      if (won)
      {
        ammountWon++;
        wonArray[j] = true;
        if (ammountWon == 1)
        {
          printf("\nPart one:\n\nBoard %d wins first, in round %d!\n", j, i);
          printBoard(boards[j]);

          int score = calcRemaining(boards[j]) * draw[i];
          printf("Score: %d\n", score);
        }
        else if (ammountWon == boardCount)
        {
          printf("\nPart two:\n\nBoard %d wins last, in round %d!\n", j, i);
          printBoard(boards[j]);

          int score = calcRemaining(boards[j]) * draw[i];
          printf("Score: %d\n", score);
        }
      }
    }
  }

  return EXIT_SUCCESS;
}
