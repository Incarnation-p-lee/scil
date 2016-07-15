#ifndef HAVE_DEFINED_DEPENDS_H
#define HAVE_DEFINED_DEPENDS_H

#include "data_structure_interface.h"
#include "define.h"
#include "finite_automata.h"
#include "external.h"

#if defined LIBC

#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stddef.h>
#include <ctype.h>
#include <sys/time.h>

#define dp_memset              memset
#define dp_isalnum             isalnum
#define dp_printf              printf
#define dp_strlen              strlen
#define dp_isalpha             isalpha

#if defined DEBUG
    #define dp_malloc          memory_track_melloc
    #define dp_free            memory_track_free
#else
    #define dp_malloc          malloc
    #define dp_free            free
#endif

#endif

#endif

