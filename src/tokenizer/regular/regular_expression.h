#ifndef REGULAR_EXPRESSION_H
#define REGULAR_EXPRESSION_H

#if defined DEBUG
    #define REGULAR_RANG_EXPAND_PRINT(x)           regular_range_expand_print(x)
    #define REGULAR_OPT_AND_INSERT_PRINT(x)        regular_opt_and_insert_print(x)
    #define REGULAR_CVT_TO_REVERSE_POLISH_PRINT(x) regular_convert_to_reverse_polish_print(x)
#else
    #define REGULAR_RANG_EXPAND_PRINT(x)
    #define REGULAR_OPT_AND_INSERT_PRINT(x)
    #define REGULAR_CVT_TO_REVERSE_POLISH_PRINT(x)
#endif

#endif

