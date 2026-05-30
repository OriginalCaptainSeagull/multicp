#include "prots.h"
#include "api.h"
#include "types.h"

int main (int argc, char *argv[])
{
  if (argc < 3)
  {
    fprintf (stderr, "Incorrect usage of %s!\nExample: %s [FILES...] -d[DESTINATION...] [FILES...]\n", argv[0], argv[0]);
    return 1;
  }
  int dest_arg = 0;
  for (unsigned i = 1; i < argc; ++i)
  {
    if (!dest_arg && search_for_dest (argv[i]) == 0)
      dest_arg = i;
    else if (dest_arg && search_for_dest (argv[i]) == 0)
    {
      fprintf (stderr, "Incorrect usage of %s!\nExample: %s [FILES...] -d[DESTINATION...] [FILES...]\n", argv[0], argv[0]);
    }
  }
  if (dest_arg == 0)
  {
    fprintf (stderr, "Couldn\'t find destinantion!\nExample: %s [FILES...] -d[DESTINATION] [FILES...]\n", argv[0]);
    return 1;
  }

  for (unsigned i = 1; i < argc; ++i)
  {
    if (i == dest_arg)
      continue;
    if (copy_file (argv[i], argv[dest_arg]))
      return 1;
  }
  return 0;
}
