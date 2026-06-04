#include "errors.h"
#include "colors.h"
#include "api.h"
#include "types.h"
#ifndef STD_C
#define STD_C
#include <stdlib.h>
#endif

void usage_error (const char* restrict error)
{
  fprintf (stderr, COLOR_RED);
  fprintf (stderr, "multicp: error: ");
  fprintf (stderr, COLOR_RESET);
  fprintf (stderr, "%s\n", error);
  exit (EXIT_FAILURE);
  return;
}

int check_number_of_args (int argc)
{
  if (argc < 3)
  {
    // checks if the number of specified arguments
    // is not enough
    usage_error ("Insufficient arguments.\nExample: multicp FILES... -dDESTINATION FILES...");
    return -1;
  }
  return 0;
}

int search_for_dest (const char *arg)
{
  static const char *const flag = "-d";
  for (unsigned i = 0; i < 2; ++i)
  {
    if (arg[i] != flag[i])
      return -1;
    // if the argument doesn't start with "-d"
    // then it is considered a file
  }
  return 0;
}

int determine_dest (char *argv[], int argc)
{
  int dest_arg = 0;
  for (unsigned i = 1; i < argc; ++i)
  {
    if (!dest_arg && search_for_dest (argv[i]) == 0)
      dest_arg = i;
    else if (dest_arg && search_for_dest (argv[i]) == 0)
    {
      // if the user specified way too many destinations (>1)
      usage_error ("Too many destinations.\nExample: multicp FILES... -dDESTINATION FILES...");
      return -1;
    }
  }
  return dest_arg;
}
