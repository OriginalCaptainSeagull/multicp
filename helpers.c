#include "types.h"
#include "api.h"
#include <stdlib.h>
#include <string.h>

char *concat (const char *first, const char *second)
{
  unsigned i = 1;
  for (unsigned j = 0; first[j]; ++j, ++i);
  for (unsigned j = 0; second[j]; ++i, ++j);

  char *result = calloc (i + 2, sizeof (char));

  strcat (result, second);
  for (i = 0; second[i]; ++i);
  result[i] = '/';
  strcat (result, first);
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
  dest = concat (src, dest);
  file source, destination;
  const perms permissions = O_CREAT | O_WRONLY | O_TRUNC;
  mode_t metadata = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  ssize_t bytes_read;
  char buff[BUFFSIZE];

  source = open (src, O_RDONLY);
  destination = open (dest, permissions, metadata);
  if (!source)
  {
    write (stderr, "couldn\'t open ", 15);
    unsigned i;
    for (i = 0; src[i]; ++i);
    write (stderr, src, i + 1);
    write (stderr, " file. check if the name is correct.\n", 38);
    return 1;
  }
  if (!destination)
  {
    write (stderr, "couldn\'t open ", 15);
    unsigned i = 0;
    for (; dest[i]; ++i);
    write (stderr, dest, i + 1);
    write (stderr, " file. check if the name is correct.\n", 38);
  }
  while ((bytes_read = read (source, buff, BUFFSIZE)) > 0)
  {
    if (write (destination, buff, bytes_read) == 0)
      break;
  }
  close (source);
  close (destination);
  free (dest);
  return 0;
}
