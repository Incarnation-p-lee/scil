#ifndef HAVE_DEFINED_TOKEN_H
#define HAVE_DEFINED_TOKEN_H

#define TOKEN_IDTR_LEN           256

typedef enum lexeme              e_lexeme_t;
typedef enum token_language_type e_token_language_t;
typedef struct token             s_token_t;

enum lexeme {
    LEX_HEAD,
    LEX_KWRD,  // keyword
    LEX_OPTR,  // operator
    LEX_IDTR,  // identifier
    LEX_CNST,  // constant
    LEX_PCTT,  // punctuation
};

enum token_language_type {
    TK_LANG_C,
};


struct token_keyword {

};

struct token_identifer {
    char name[TOKEN_IDTR_LEN];
};

struct token_operator {
};

struct token_constant {
};

struct token_punctuation {
};

struct token {
    e_lexeme_t             type;
    void                   *data;
    s_doubly_linked_list_t list;
};

#endif

