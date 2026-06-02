#include "signatures.h"
#include "api.h"
#include "types.h"

int main (int argc, char *argv[])
{
  if (check_number_of_args (argc))
    return 1;

  int dest_arg = 0;
  dest_arg = determine_dest (argv, argc);
  if (dest_arg == -1)
    return 1;
  if (dest_arg == 0)
  {
    usage_error ("Couldn\'t find destinantion!\nExample: multicp FILES... -dDESTINATION FILES...\n");
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
