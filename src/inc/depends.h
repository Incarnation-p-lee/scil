#ifndef HAVE_DEFINED_DEPENDS_H
#define HAVE_DEFINED_DEPENDS_H

#include "data_structure_interface.h"
#include "external.h"

#if defined LIBC

#include <unistd.h>
#include <limits.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stddef.h>
#include <sys/time.h>

#if defined DEBUG
    #define dp_assert              assert
#else
    #define dp_assert(x)
#endif

#endif

#endif

