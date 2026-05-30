#ifndef MULTICP
#define MULTICP
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#ifndef BUFFSIZE
#define BUFFSIZE 1024
#endif
enum
{
  stdin = 0,
  stdout = 1,
  stderr = 2,
};
typedef int file;
typedef int perms;
#endif
