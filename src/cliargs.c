#include "errors.h"
#include "colors.h"
#include "api.h"
#include "types.h"
static const char* const dir = "-d";
// static const char* const ver = "-v";

void usage_error (const char* restrict error)
{
  fprintf (stderr, COLOR_RED);
  fprintf (stderr, "multicp: error: ");
  fprintf (stderr, COLOR_RESET);
  fprintf (stderr, "%s\n", error);
  exit (EXIT_FAILURE);
  return;
}

int fill_with_files (char **argv, char **files, int argc)
{
  int filecount = 0;
  for (unsigned i = 1; i < argc; ++i)
  {
    if (strncmp (argv[i], dir, 2))
    {
      files[filecount] = argv[i];
      ++filecount;
    }
  }
  return filecount;
}

int fill_with_destinations (char **argv, char **destinations, int argc)
{
  int dircount = 0;
  for (unsigned i = 1; i < argc; ++i)
  {
    if (strncmp (argv[i], dir, 2) == 0)
    {
      destinations[dircount] = argv[i];
      ++dircount;
    }
  }
  return dircount;
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
/*
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
*/
