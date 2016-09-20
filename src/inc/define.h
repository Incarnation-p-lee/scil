#ifndef HAVE_DEFINED_DEFINE_H
#define HAVE_DEFINED_DEFINE_H

#define true                   1
#define false                  0
#define PTR_SIZE_OF(e, s)      ((uint32)(e - s) + 1)

#define PTR_SENTINEL           (void *)0xA5
#define TO_STRING(x)           (#x)
#define CONTAINS_OF(addr, type, member) \
                               (void *)((ptr_t)(addr) - (ptr_t)(&((type *)0)->member))
#define ARRAY_SIZE_OF(y)       (sizeof(y) / sizeof(y[0]))

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

