#include "errors.h"
#include "colors.h"
#include "api.h"
#include "types.h"

void usage_error (const char* restrict error)
{
  fprintf (stderr, COLOR_RED);
  fprintf (stderr, "multicp: error: ");
  fprintf (stderr, COLOR_RESET);
  fprintf (stderr, "%s\n", error);

  return;
}

int check_number_of_args (int argc)
{
  if (argc < 3)
  {
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
      usage_error ("Too many destinations.\nExample: multicp FILES... -dDESTINATION FILES...");
      return -1;
    }
  }
  return dest_arg;
}
