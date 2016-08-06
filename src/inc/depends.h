#ifndef HAVE_DEFINED_DEPENDS_H
#define HAVE_DEFINED_DEPENDS_H

#if defined LIBC

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include <stdarg.h>

#define dp_memset              memset
#define dp_isalnum             isalnum
#define dp_printf              printf
#define dp_strlen              strlen
#define dp_isalpha             isalpha
#define dp_fopen               fopen
#define dp_fclose              fclose
#define dp_exit                exit
#define dp_vfprintf            vfprintf
#define dp_sprintf             sprintf
#define dp_va_list             va_list
#define dp_va_start            va_start
#define dp_va_end              va_end
#define dp_fflush              fflush

#if defined DEBUG
    #define dp_malloc          memory_track_malloc
    #define dp_free            memory_track_free
#else
    #define dp_malloc          malloc
    #define dp_free            free
#endif


#include "data_structure_interface.h"
#include "define.h"
#include "finite_automata.h"
#include "external.h"


#endif

#endif

