#ifndef HAVE_DEFINED_DEPENDS_H
#define HAVE_DEFINED_DEPENDS_H

#include "data_structure_interface.h"
#include "finite_automata.h"
#include "external.h"

#define true                   1
#define false                  0

#if defined LIBC

#include <unistd.h>
#include <limits.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stddef.h>
#include <ctype.h>
#include <sys/time.h>

#if defined DEBUG
    #define dp_assert          assert
#else
    #define dp_assert(x)
#endif

#define dp_malloc              malloc
#define dp_free                free
#define dp_memset              memset
#define dp_isalnum             isalnum

#endif

#endif

