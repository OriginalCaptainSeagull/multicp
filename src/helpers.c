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
/*
char *get_filename (char *path)
{
  char *newpath = strrchr (path, '@');
  if (newpath)
  {
    printf ("%c", newpath[0]);
    newpath[0] = '\0';
    return newpath + 1;
  }
  newpath = strrchr (path, '/');
  if (newpath)
    return newpath + 1;
  return newpath;
}
*/
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
  int err_check = 0;
  dest += 2; // getting rid of the "-d"
  dest = format_dest (src, dest);
  file source, destination;
  const perms permissions = O_CREAT | O_WRONLY | O_TRUNC; // ask to get enough POWER to
  // create (if the file doesn't exist), write to it and clear the content if the file
  // already exists
  mode_t metadata = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  // create a file with rw-rw-rw- permissions if one doesn't exist
  ssize_t bytes_read;
  static char buff[BUFFSIZE];

  source = open (src, O_RDONLY);
  if (source == -1)
  {
    print_error ("Couldn\'t create a copy of a file ", src);
    err_check = -1;
    goto quit;
  }
  destination = open (dest, permissions, metadata);
  if (destination == -1)
  {
    print_error ("Couldn\'t create a copy of a file ", dest );
    // skill issue of your OS imo :/
    err_check = -1;
    goto quitall;
  }

  while ((bytes_read = read (source, buff, BUFFSIZE)) > 0)
  {
    err_check = write (destination, buff, bytes_read);
    if (err_check == -1)
    {
      print_error ("An error occured while copying ", src);
      break;
    }
  }
quitall:
  free (dest);
quit:
  close (source);
  close (destination);
  printf ("%d", err_check);
  if (err_check == -1)
    return 1;
  return 0;
}
