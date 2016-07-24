#ifndef HAVE_DEFINED_DEFINE_H
#define HAVE_DEFINED_DEFINE_H

#define true                   1
#define false                  0

#define PTR_SENTINEL           (void *)0xA5
#define TO_STRING(x)           (#x)

#define assert_caution(exp)    do {                                                                  \
                                   if (!(exp)) {                                                     \
                                       assert_print_caution(#exp, __FILE__, __FUNCTION__, __LINE__); \
                                   }                                                                 \
                               } while (false)

#if defined DEBUG
    #define assert_exit(exp)   do {                                                               \
                                   if (!(exp)) {                                                  \
                                       assert_print_exit(#exp, __FILE__, __FUNCTION__, __LINE__); \
                                   }                                                              \
                               } while (false)
#else
    #define assert_exit(exp)
#endif

#endif

