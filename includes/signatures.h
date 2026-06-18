#ifndef PROTOTYPES
#define PROTOTYPES
int search_for_dest (char *arg);
int copy_file (char *src, char *dest);
int check_number_of_args (int args);
void print_error (const char* error, const char *filename);
int fill_with_files (char **argv, char **files, int argc);
int fill_with_destinations (char **argv, char **destinations, int argc);
void usage_error (const char* restrict error);
#endif
