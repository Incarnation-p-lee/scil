#ifndef HAVE_DEFINED_TOKEN_H
#define HAVE_DEFINED_TOKEN_H

#define TOKEN_IDTR_LEN               256
#define TK_C_OPTR_SZ                 8
#define TK_C_OPTR_DUAL(x, y)         (((x) << TK_C_OPTR_SZ) + (y))
#define TK_C_OPTR_TRIPLE(x, y, z)    ((TK_C_OPTR_DUAL(x, y) << TK_C_OPTR_SZ) + (z))
#define TK_LANG_DUAL(x, y)           TK_C_OPTR_DUAL(x, y)

#include "token_language_c.h"

typedef enum token_lexeme            e_token_lexeme_t;
typedef struct token                 s_token_t;

enum token_lexeme {
    TK_LEX_HEAD,
    TK_LEX_KWRD,  // keyword
    TK_LEX_OPTR,  // operator
    TK_LEX_IDTR,  // identifier
    TK_LEX_CNST,  // constant
    TK_LEX_PCTT,  // punctuation
};

struct token {
    e_token_lexeme_t       type;
    void                   *data;
    s_doubly_linked_list_t list;
};

#endif

