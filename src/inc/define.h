#ifndef HAVE_DEFINED_DEFINE_H
#define HAVE_DEFINED_DEFINE_H

#define true                   1
#define false                  0

#define NULL_CHAR              ((char)0)
#define PTR_SIZE_OF(e, s)      ((uint32)(e - s) + 1)
#define NULL_PTR_P(ptr)        (ptr == NULL ? true : false)
#define NON_NULL_PTR_P(ptr)    (ptr != NULL ? true : false)

#define PTR_SENTINEL           (void *)0xA5
#define TO_STRING(x)           (#x)
#define ARRAY_SIZE_OF(y)       (sizeof(y) / sizeof(y[0]))

#define RETURN_FIR_IF_EQ(r, v) if ((r) == (v)) { \
                                   return r;     \
                               }

#define RETURN_FIR_IF_NE(r, v) if ((r) != (v)) { \
                                   return r;     \
                               }

#define CONTAINS_OF(addr, type, member) \
                               (void *)((ptr_t)(addr) - (ptr_t)(&((type *)0)->member))
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
    #define inline             __attribute__((noinline))
#else
    #define assert_exit(exp)
    // #define inline             __attribute__((noinline))
    #define inline             inline
#endif

#endif

