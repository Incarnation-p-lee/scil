#ifndef HAVE_DEFINED_TOKEN_H
#define HAVE_DEFINED_TOKEN_H

#define TOKEN_IDTR_LEN                 256
#define TK_OPTR_SZ                     8
#define TK_OPTR_DUAL(x, y)             (((x) << TK_OPTR_SZ) + (y))
#define TK_OPTR_TRIPLE(x, y, z)        ((TK_OPTR_DUAL(x, y) << TK_OPTR_SZ) + (z))
#define TK_LANG_DUAL(x, y)             TK_OPTR_DUAL(x, y)

typedef enum lexeme                    e_lexeme_t;
typedef enum token_lang_c_optr_type    e_token_lang_c_optr_type_t;
typedef enum token_lang_c_keyword_type e_token_lang_c_keyword_type_t;
typedef enum token_low_letter          e_token_low_letter_t;
typedef struct token                   s_token_t;
typedef struct token_lang_c_optr       s_token_lang_c_optr_t;
typedef struct token_lang_c_idtr       s_token_lang_c_idtr_t;

enum lexeme {
    LEX_HEAD,
    LEX_KWRD,  // keyword
    LEX_OPTR,  // operator
    LEX_IDTR,  // identifier
    LEX_CNST,  // constant
    LEX_PCTT,  // punctuation
};

struct token {
    e_lexeme_t             type;
    void                   *data;
    s_doubly_linked_list_t list;
};


/* The C language token definition */

enum token_lang_c_optr_type {
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

static const char *lang_c_keyword[] = {
//   123456789
    "asm",
    "auto",
    "break",
    "case",
    "char",
    "const",
    "continue",
    "default",
    "double",
    "do",
    "else",
    "enum",
    "extern",
    "float",
    "for",
    "goto",
    "if",
    "inline",
    "int",
    "long",
    "register",
    "restrict",
    "return",
    "short",
    "signed",
    "sizeof",
    "static",
    "struct",
    "switch",
    "typedef",
    "union",
    "unsigned",
    "void",
    "volatile",
    "while",
};

enum token_lang_c_keyword_type {
    TK_IDTR_C_ASM,
    TK_IDTR_C_AUTO,
    TK_IDTR_C_BREAK,
    TK_IDTR_C_CASE,
    TK_IDTR_C_CHAR,
    TK_IDTR_C_CONST,
    TK_IDTR_C_CONTINUE,
    TK_IDTR_C_DEFAULT,
    TK_IDTR_C_DOUBLE,
    TK_IDTR_C_DO,
    TK_IDTR_C_ELSE,
    TK_IDTR_C_ENUM,
    TK_IDTR_C_EXTERN,
    TK_IDTR_C_FLOAT,
    TK_IDTR_C_FOR,
    TK_IDTR_C_GOTO,
    TK_IDTR_C_IF,
    TK_IDTR_C_INLINE,
    TK_IDTR_C_INT,
    TK_IDTR_C_LONG,
    TK_IDTR_C_REGISTER,
    TK_IDTR_C_RESTRICT,
    TK_IDTR_C_RETURN,
    TK_IDTR_C_SHORT,
    TK_IDTR_C_SIGNED,
    TK_IDTR_C_SIZEOF,
    TK_IDTR_C_STATIC,
    TK_IDTR_C_STRUCT,
    TK_IDTR_C_SWITCH,
    TK_IDTR_C_TYPEDEF,
    TK_IDTR_C_UNION,
    TK_IDTR_C_UNSIGNED,
    TK_IDTR_C_VOID,
    TK_IDTR_C_VOLATILE,
    TK_IDTR_C_WHILE,
    TK_IDTR_C_NONE,

    TK_IDTR_C_FIRST = TK_IDTR_C_ASM,
    TK_IDTR_C_LAST  = TK_IDTR_C_NONE,
};

struct token_lang_c_optr {
    e_token_lang_c_optr_t type;
};

struct token_lang_c_idtr {
    bool is_keyword;
    union {
        e_token_lang_c_keyword_type_t type;
        char *name;
    };
};

struct token_lang_c_cnst {


};


#endif

