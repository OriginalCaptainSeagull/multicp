#include "types.h"
#include "api.h"
#include <stdlib.h>
#include <string.h>
// mo/cha\0
// mo^cha\0
char* get_filename (const char *path)
{
  unsigned length = 0;
  int slashpos = -1;
  for (unsigned i = 0; path[i]; ++i, ++length)
  {
    if (path[i] == '/')
      slashpos = i;
  }
  if (slashpos == -1)
    return path;
  path += slashpos + 1;
  return path;
}

char *format_dest (const char *raw_src, const char *raw_dest)
{
  char *src = get_filename (raw_src);
  unsigned i = 1;
  for (unsigned j = 0; src[j]; ++i, ++j);
  for (unsigned j = 0; raw_dest[j]; ++i, ++j);

  char *result = calloc (i + 2, sizeof (char));

  strcat (result, raw_dest);
  for (i = 0; raw_dest[i]; ++i);
  if (raw_dest[i - 1] != '/')
    result[i] = '/';
  strcat (result, src);
  return result;
}

int search_for_dest (const char *arg)
{
  static const char *const flag = "-d";
  for (unsigned i = 0; i < 2; ++i)
  {
    if (arg[i] != flag[i])
      return -1;
  }
  return 0;
}

int copy_file (const char *src, const char *dest)
{
  dest += 2;
  dest = format_dest (src, dest);
  file source, destination;
  const perms permissions = O_CREAT | O_WRONLY | O_TRUNC;
  mode_t metadata = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  ssize_t bytes_read;
  char buff[BUFFSIZE];

  source = open (src, O_RDONLY);
  destination = open (dest, permissions, metadata);
  if (!source)
  {
    fprintf  (stderr, "Couldn\'t create a copy of a file %s: ", src);
    perror (NULL);
    free (dest);
    return 1;
  }
  if (!destination)
  {
    fprintf (stderr, "Couldn\'t create a copy of a file %s: ", src);
    perror (NULL);
    close (source);
    free (dest);
    return 1;
  }
  int err_check;
  while ((bytes_read = read (source, buff, BUFFSIZE)) > 0)
  {
    err_check = write (destination, buff, bytes_read);
    if (err_check == -1)
    {
      fprintf (stderr, "An error occured while copying %s: ", src);
      perror (NULL);
      break;
    }
  }
  close (source);
  close (destination);
  free (dest);
  if (err_check == -1)
    return 1;
  return 0;
}
