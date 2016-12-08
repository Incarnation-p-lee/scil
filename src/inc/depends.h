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
#define dp_isdigit             isdigit
#define dp_isspace             isspace
#define dp_printf              printf
#define dp_feof                feof
#define dp_fgets               fgets
#define dp_strlen              strlen
#define dp_isalpha             isalpha
#define dp_fopen               fopen
#define dp_fclose              fclose
#define dp_fread               fread
#define dp_exit                exit
#define dp_fprintf             fprintf
#define dp_vfprintf            vfprintf
#define dp_sprintf             sprintf
#define dp_va_list             va_list
#define dp_va_start            va_start
#define dp_va_end              va_end
#define dp_fflush              fflush
#define dp_memcpy              memcpy
#define dp_strlen              strlen
#define dp_strcpy              strcpy
#define dp_strchr              strchr
#define dp_strrchr             strrchr
#define dp_strcmp              strcmp
#define dp_strncmp             strncmp

#if defined DEBUG
    #define dp_malloc          memory_track_malloc
    #define dp_realloc         memory_track_realloc
    #define dp_free            memory_track_free
#else
    #define dp_malloc          malloc
    #define dp_realloc         realloc
    #define dp_free            free
#endif

#include "data_structure_interface.h"
#include "define.h"
#include "external.h"

#endif

#endif

