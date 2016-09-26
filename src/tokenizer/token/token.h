#ifndef HAVE_DEFINED_TOKEN_H
#define HAVE_DEFINED_TOKEN_H

typedef enum token_lexeme  e_token_lexeme_t;
typedef struct token       s_token_t;
typedef struct token_file  s_token_file_t;

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

struct token_file {
    char                   *filename;
    s_token_t              *token_head;
    s_doubly_linked_list_t list;
};


#endif

