#ifndef PROTOTYPES
#define PROTOTYPES
int search_for_dest (char *arg);
int copy_file (char *src, char *dest);
int check_number_of_args (int args);
int determine_dest (char *argv[], int argc);
void usage_error (const char* restrict error);
#endif
