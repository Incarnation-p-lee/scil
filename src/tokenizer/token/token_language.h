#ifndef HAVE_DEFINED_TOKEN_LANG_C
#define HAVE_DEFINED_TOKEN_LANG_C

#if defined DEBUG
    #define TK_LANGUAGE_C_PRINT(tk) token_language_c_print(tk)
#else
    #define TK_LANGUAGE_C_PRINT(tk)
#endif

#include "token_language_c.h"

#endif

