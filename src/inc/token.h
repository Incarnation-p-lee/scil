#ifndef HAVE_DEFINED_TOKEN_H
#define HAVE_DEFINED_TOKEN_H

#define TK_DOUBLE_QUOTE        ((char)0x22)    // '"'
#define TK_SLASH               '/'
#define TK_STAR                '*'
#define TK_NEWLINE             '\n'
#define TK_FILE_SPLIT          '.'
#define TK_SENTINEL            ((char)0xa4)

#include "token_language_c.h"

typedef enum token_type        e_tk_type_t;
typedef struct token           s_tk_t;

enum token_type {
    TK_LEX_HEAD,
    TK_LEX_KWRD,  // keyword
    TK_LEX_OPTR,  // operator
    TK_LEX_IDTR,  // identifier
    TK_LEX_CNST,  // constant
    TK_LEX_PCTT,  // punctuation
};

struct token {
    e_tk_type_t            type;
    void                   *data;
    uint32                 line_nmbr;
    s_doubly_linked_list_t list;
};

#endif

