#ifndef HAVE_DEFINED_TOKEN_H
#define HAVE_DEFINED_TOKEN_H

#define TOKEN_IDTR_LEN           256
#define TK_OPTR_SZ               8
#define TK_OPTR_DUAL(x, y)       (((x) << TK_OPTR_SZ) + (y))
#define TK_OPTR_TRIPLE(x, y, z)  ((TK_OPTR_DUAL(x, y) << TK_OPTR_SZ) + (z))
#define TK_LANG_DUAL(x, y)       TK_OPTR_DUAL(x, y)

typedef enum lexeme              e_lexeme_t;
typedef enum token_lang_type     e_token_lang_type_t;
typedef enum token_lang_c_optr   e_token_lang_c_optr_t;
typedef struct token             s_token_t;
typedef struct token_c_optr      s_token_c_optr_t;

enum lexeme {
    LEX_HEAD,
    LEX_KWRD,  // keyword
    LEX_OPTR,  // operator
    LEX_IDTR,  // identifier
    LEX_CNST,  // constant
    LEX_PCTT,  // punctuation
};

enum token_lang_c_optr {
    TK_OPTR_NOT = '!',  // ! 0x21
    TK_OPTR_MOD = '%',  // % 0x25
    TK_OPTR_AND = '&',  // & 0x26
    TK_OPTR_MUL = '*',  // * 0x2A
    TK_OPTR_ADD = '+',  // + 0x2B
    TK_OPTR_SUB = '-',  // - 0x2D
    TK_OPTR_REF = '.',  // . 0x2E
    TK_OPTR_DIV = '/',  // / 0x2F
    TK_OPTR_CLN = ':',  // : 0x3A
    TK_OPTR_LT  = '<',  // < 0x3C
    TK_OPTR_EQ  = '=',  // = 0x3D
    TK_OPTR_GT  = '>',  // > 0x3E
    TK_OPTR_QST = '?',  // ? 0x3F
    TK_OPTR_XOR = '^',  // ^ 0x5E
    TK_OPTR_OR  = '|',  // | 0x7C
    TK_OPTR_NEG = '~',  // ~ 0x7E

    TK_OPTR_UNARY_LIMIT = TK_OPTR_NEG + 1，

    TK_OPTR_GE      = TK_OPTR_DUAL(TK_OPTR_GT, TK_OPTR_EQ),               // >=
    TK_OPTR_LE      = TK_OPTR_DUAL(TK_OPTR_LT, TK_OPTR_EQ),               // <=
    TK_OPTR_ORE     = TK_OPTR_DUAL(TK_OPTR_OR, TK_OPTR_OR)                // |=
    TK_OPTR_ADDE    = TK_OPTR_DUAL(TK_OPTR_ADD, TK_OPTR_EQ),              // +=
    TK_OPTR_SUBE    = TK_OPTR_DUAL(TK_OPTR_SUB, TK_OPTR_EQ),              // -=
    TK_OPTR_MULE    = TK_OPTR_DUAL(TK_OPTR_MUL, TK_OPTR_EQ),              // *=
    TK_OPTR_ANDE    = TK_OPTR_DUAL(TK_OPTR_AND, TK_OPTR_EQ),              // &=
    TK_OPTR_XORE    = TK_OPTR_DUAL(TK_OPTR_XOR, TK_OPTR_EQ),              // ^=
    TK_OPTR_DIVE    = TK_OPTR_DUAL(TK_OPTR_DIV, TK_OPTR_EQ),              // /=
    TK_OPTR_SHT_L   = TK_OPTR_DUAL(TK_OPTR_LT, TK_OPTR_LT),               // <<
    TK_OPTR_SHT_R   = TK_OPTR_DUAL(TK_OPTR_GT, TK_OPTR_GT),               // >>
    TK_OPTR_REF_PTR = TK_OPTR_DUAL(TK_OPTR_SUB, TK_OPTR_GT),              // ->
    TK_OPTR_SHTE_L  = TK_OPTR_TRIPLE(TK_OPTR_LT, TK_OPTR_LT, TK_OPTR_EQ), // <<=
    TK_OPTR_SHTE_R  = TK_OPTR_TRIPLE(TK_OPTR_GT, TK_OPTR_GT, TK_OPTR_EQ), // >>=
};

enum token_lang_type {
    TK_LANG_C    = 'c',
    TK_LANG_CPP  = TK_OPTR_TRIPLE('c', 'p', 'p'),

};


struct token_keyword {

};

struct token_identifer {
    char name[TOKEN_IDTR_LEN];
};

struct token_c_optr {
    e_token_lang_c_optr_t type;
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

