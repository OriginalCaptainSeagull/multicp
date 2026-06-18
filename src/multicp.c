#include "signatures.h"
#include "api.h"
#include "types.h"
#define ARGS (argc - 1)

int main (int argc, char *argv[])
{
  if (check_number_of_args (argc)) // check cliargs.c
    return 1;

  char** files = (char**) calloc (ARGS, sizeof (char**));
  if (files == NULL)
    return 1;

  char** destinations = (char**) calloc (ARGS, sizeof (char**));
  if (destinations == NULL)
  {
    free (files);
    return 1;
  }

  int filecount = fill_with_files (argv, files, argc);
  if (!filecount)
  {
    free (files);
    free (destinations);
    usage_error ("No files found!");
    return 1;
  }

  int dircount = fill_with_destinations (argv, destinations, argc);
  if (!dircount)
  {
    free (files);
    free (destinations);
    usage_error ("Nice directories dude!");
    return 1;
  }

  unsigned err_check;
  for (unsigned i = 0; i < filecount; ++i)
  {
    for (unsigned j = 0; j < dircount; ++j)
      err_check = copy_file (files[i], destinations[j]);
    if (err_check)
      goto end;
  }

end:
  free (files);
  free (destinations);
  return err_check;
}
