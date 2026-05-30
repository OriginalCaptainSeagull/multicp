#include "prots.h"
#include "api.h"
#include "types.h"

int main (int argc, char *argv[])
{
  if (argc < 3)
  {
    write (stderr, "usage error!\nmulticp [FILES...] -d[DESTINATIONS...]\n", 53);
    return 1;
  }
  int dest_arg = 0;
  for (unsigned i = 1; i < argc; ++i)
  {
    if (!dest_arg && search_for_dest (argv[i]) == 0)
      dest_arg = i;
    else if (dest_arg && search_for_dest (argv[i]) == 0)
    {
      write (stderr, "usage error: multiple destinations!", 36);
    }
  }
  if (dest_arg == 0)
  {
    write (stderr, "usage error!\ndestination not found. use flag -d to point it out!\n", 66);
    return 1;
  }

  for (unsigned i = 1; i < argc; ++i)
  {
    if (i == dest_arg)
      continue;
    if (copy_file (argv[i], argv[dest_arg]))
    {
      write (stderr, "an unknown error occured!\n", 27);
      return 1;
    }
  }
  return 0;
}
