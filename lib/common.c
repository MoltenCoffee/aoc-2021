#include "common.h"

static size_t getFilesize(FILE *file)
{
  fseek(file, 0L, SEEK_END);
  size_t fileSize = ftell(file);
  rewind(file);

  return fileSize;
}

char *readFile(const char *path)
{
  FILE *file = fopen(path, "r");
  if (file == NULL)
    fprintf(stderr, "Could not open provided file. Check spelling or permissions for %s\n", path);

  size_t fileSize = getFilesize(file);

  // Allocate memory for file contents
  char *buffer = (char *)malloc(fileSize + 1);
  if (buffer == NULL)
    fprintf(stderr, "Could not allocate memory to read file %s\n", path);

  // Read file into memory
  size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
  if (bytesRead < fileSize)
    fprintf(stderr, "Could not read file %s into memory\n", path);

  // Add null byte
  buffer[bytesRead] = '\0';

  fclose(file);
  return buffer;
}

char* readLine(char* buffer)
{
  return strtok(buffer, "\n");
}
