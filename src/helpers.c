#include "colors.h"
#include "types.h"
#include "api.h"
#include "signatures.h"
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

void print_error (const char *error, const char *filename)
{
  fprintf (stderr, COLOR_RED);
  fprintf (stderr, "multicp error: ");
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
  file source, destination;
  int error_check = 0;
  int bytes_read;
  static char *buffer[BUFFSIZE];

  source = open (src, O_RDONLY);
  if (source == -1)
  {
    print_error ("Couldn\'t open the file ", src);
    return -1;
  }

  dest = format_dest (src, dest);
  destination = open (dest, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP | S_IROTH | S_IWOTH);
  free (dest);
  if (destination == -1)
  {
    print_error ("Couldn\'t create copy of file", NULL);
    close (source);
    return -1;
  }

  while (bytes_read != 0)
  {
    bytes_read = read (source, buffer, BUFFSIZE);
    if (bytes_read == -1)
    {
      error_check = -1;
      break;
    }
    error_check = write (destination, buffer, bytes_read);
    if (error_check == -1)
      break;
  }

  close (destination);
  close (source);
  return error_check;
}
