#ifndef HAVE_DEFINED_DEFINE_H
#define HAVE_DEFINED_DEFINE_H

#define true                   1
#define false                  0

#if defined DEBUG
    #define assert(exp)        ((exp) || complain_assert(#exp, __FILE__, \
                                   __FUNCTION__, __LINE__))
#else
    #define assert(exp)
#endif

#endif

