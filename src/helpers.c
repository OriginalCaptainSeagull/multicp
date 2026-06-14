#include "colors.h"
#include "types.h"
#include "api.h"
#include "signatures.h"
#ifndef STD_C
#define STD_C
#include <stdlib.h>
#include <string.h>
#endif

void print_error (const char *error, const char *filename)
{
  fprintf (stderr, COLOR_RED);
  fprintf (stderr, "multicp: error: ");
  fprintf (stderr, COLOR_RESET);
  fprintf (stderr, "%s%s: %s\n", error, filename, strerror(errno));
  return;
  // a function needed to only DISPLAY the errors
  // which is the only difference from it's alternative
  // from src/cliargs.c
}

char* get_filename (char *path)
{
  unsigned length = 0;
  int slashpos = -1; // position of the last slash symbol
  int atpos = -1; // i mean this -> @. it's needed to indicate renaming
  for (unsigned i = 0; path[i]; ++i, ++length)
  {
    if (path[i] == '/')
      slashpos = i;
    // looks for the LAST slash in the argument
    if (path[i] == '@')
    {
      // looks for ats in the argument.
      atpos = i;
      path[i] = '\0';
      // if detects one, it gets replaced with nullterm
      // so the syscall open() will not try to open a file like
      // "file@newname"
      // but instead will open a file named "file"
    }
  }
  if (atpos > 0)
    return (path + atpos + 1); // return new file name
  else if (slashpos > 0)
    return (path + slashpos + 1); // returns the name
  return path;
}

char *format_dest (char *raw_src, char *raw_dest)
{
  raw_dest += 2;
  char *src = get_filename (raw_src);
  unsigned i = 1;
  for (unsigned j = 0; src[j]; ++i, ++j);
  for (unsigned j = 0; raw_dest[j]; ++i, ++j);

  char *result = (char *) calloc (i + 2, sizeof (char));

  strcat (result, raw_dest);
  for (i = 0; raw_dest[i]; ++i);
  result[i] = '/';
  strcat (result, src);
  return result;
}

int copy_file (char *src, char *dest)
{
  FILE *source = NULL;
  FILE *destination = NULL;
  static char buffer[BUFFSIZE];
  int err_check, bytes_read;
  err_check = 0;

  dest = format_dest (src, dest); // formats the destination.
  // for example if src = ../path/to/myfile and dest = -d../another/path/
  // then format_dest() makes dest = ../another/path/myfile

  source = fopen (src, "rb"); // read binary
  if (source == NULL)
  {
    print_error ("Couldn\'t create a copy of file ", src);
    err_check = -1;
  }
  destination = fopen (dest, "w+b");
  if (destination == NULL)
  {
    fclose (source);
    print_error ("Couldn\'t create a copy of file ", src);
    err_check = -1;
  }

  free (dest); // frees the memory dest is pointing at
  // cuz it's actually allocated on the heap not stack
  if (err_check == -1)
    goto end;

  while (bytes_read != EOF)
  {
    if (fgets (buffer, BUFFSIZE, source) == NULL)
      break;
    bytes_read = fputs (buffer, destination);
  }
end:
  fclose (source);
  fclose (destination);
  return (err_check != 0) ? 1 : err_check;
}
