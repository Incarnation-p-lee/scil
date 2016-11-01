#ifndef HAVE_DEFINED_TOKEN_DEFINE_H
#define HAVE_DEFINED_TOKEN_DEFINE_H

#if defined DEBUG
    #define TK_LANGUAGE_C_PRINT(tk) token_language_c_print(tk)
#else
    #define TK_LANGUAGE_C_PRINT(tk)
#endif

#define TK_C_KYWD_HOLDER            '-'
#define TK_C_KYWD_ENCODE_SIZE       4


#endif

