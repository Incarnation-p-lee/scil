#ifndef HAVE_DEFINED_TOKEN_H
#define HAVE_DEFINED_TOKEN_H

#define TOKEN_IDTR_LEN             256
#define TK_CHAR_SZ                 8
#define TK_CHAR_MASK               ((1 << TK_CHAR_SZ) - 1)

#define TK_TO_CHAR(x)              ((x) & TK_CHAR_MASK)
#define TK_2_CHAR_JOIN(x, y)       ((TK_TO_CHAR(x) << TK_CHAR_SZ) + TK_TO_CHAR(y))
#define TK_3_CHAR_JOIN(x, y, z)    ((TK_2_CHAR_JOIN(x, y) << TK_CHAR_SZ) + TK_TO_CHAR(z))
#define TK_4_CHAR_JOIN(x, y, z, a) ((TK_3_CHAR_JOIN(x, y, z) << TK_CHAR_SZ) + TK_TO_CHAR(a))
#define TK_4_CHAR_TO_U32(s)        TK_4_CHAR_JOIN(s[0], s[1], s[2], s[3])

#define TK_DOUBLE_QUOTE            ((char)0x22)    // '"'
#define TK_SLASH                   '/'
#define TK_STAR                    '*'
#define TK_NEWLINE                 '\n'
#define TK_FILE_SPLIT              '.'
#define TK_SENTINEL                ((char)0xa4)

#include "token_language_c.h"

typedef enum token_lexeme          e_token_lexeme_t;
typedef struct token               s_token_t;

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

