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
