#ifndef TLPI_HDR_H
#define TLPI_HDR_H
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "get_num.h"
/* Prevent accidental double inclusion */
/* Type definitions used by many programs */
/* Standard I/O functions */
/* Prototypes of commonly used library functions,
plus EXIT_SUCCESS and EXIT_FAILURE constants */
/* Prototypes for many system calls */
/* Declares errno and defines error constants */
/* Commonly used string-handling functions */
/* Declares our functions for handling numeric
arguments (getInt(), getLong()) */
#include "error_functions.h"
/* Declares our error-handling functions */
typedef enum { FALSE, TRUE } Boolean;
#define min(m,n) ((m) < (n) ? (m) : (n))
#define max(m,n) ((m) > (n) ? (m) : (n))
#endif
