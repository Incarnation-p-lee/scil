#ifndef REGULAR_EXPRESSION_H
#define REGULAR_EXPRESSION_H

#define ENCODE_MASK                          ((char)0x80)
#define DECODE_MASK                        (char)(~ENCODE_MASK)

#if defined DEBUG
    #define REGULAR_RANG_RECOVER_PRINT(x)   regular_range_recover_print(x)
    #define REGULAR_OPT_AND_INSERT_PRINT(x) regular_char_and_insert_print(x)
    #define REGULAR_CVT_TO_POLISH_PRINT(x)  regular_convert_to_polish_print(x)
#else
    #define REGULAR_RANG_RECOVER_PRINT(x)
    #define REGULAR_OPT_AND_INSERT_PRINT(x)
    #define REGULAR_CVT_TO_POLISH_PRINT(x)
#endif

#endif

