#ifndef HAVE_DEFINED_TOKEN_LANGUAGE_C_H
#define HAVE_DEFINED_TOKEN_LANGUAGE_C_H

/*
 * The C language token definition 
 * Abbreviation for token type as below
 *     kywd -> keyword
 *     optr -> operator
 *     idtr -> identifier
 *     cnst -> constant
 *     pctt -> punctuation
 */
#define LANGUAGE_C_RE_IDTR           "[A-Za-z_][a-zA-Z0-9_]*"
#define LANGUAGE_C_RE_OPTR           "`+|`*|`?|-|/|`||~|^|%|!|&|.|:|=|>|<"
#define LANGUAGE_C_RE_CNST           "[0-9]+|\"[a-zA-Z0-9_ #%]+\""
#define LANGUAGE_C_RE_PCTT           "`[`],;`(`){}"

/* For Language C, 16 children size of keyword trie is enough */
#define TK_KYWD_CHILD_MAX            16
#define TK_LANG_UNMATCH             0

typedef enum token_language_c_optr_type e_token_language_c_optr_type_t;
typedef enum token_language_c_kywd_type e_token_language_c_kywd_type_t;
typedef enum token_language_c_pctt_type e_token_language_c_pctt_type_t;

typedef struct token_language_c_kywd    s_token_language_c_kywd_t;
typedef struct token_language_c_optr    s_token_language_c_optr_t;
typedef struct token_language_c_idtr    s_token_language_c_idtr_t;
typedef struct token_language_c_cnst    s_token_language_c_cnst_t;
typedef struct token_language_c_pctt    s_token_language_c_pctt_t;

enum token_language_c_optr_type {
    TK_C_OPTR_NOT = '!',                // ! 0x21
    TK_C_OPTR_MOD = '%',                // % 0x25
    TK_C_OPTR_AND = '&',                // & 0x26
    TK_C_OPTR_MUL = '*',                // * 0x2A
    TK_C_OPTR_ADD = '+',                // + 0x2B
    TK_C_OPTR_SUB = '-',                // - 0x2D
    TK_C_OPTR_REF = '.',                // . 0x2E
    TK_C_OPTR_DIV = '/',                // / 0x2F
    TK_C_OPTR_CLN = ':',                // : 0x3A
    TK_C_OPTR_LT  = '<',                // < 0x3C
    TK_C_OPTR_EQ  = '=',                // = 0x3D
    TK_C_OPTR_GT  = '>',                // > 0x3E
    TK_C_OPTR_QST = '?',                // ? 0x3F
    TK_C_OPTR_XOR = '^',                // ^ 0x5E
    TK_C_OPTR_OR  = '|',                // | 0x7C
    TK_C_OPTR_NEG = '~',                // ~ 0x7E

    TK_C_OPTR_UNARY_LIMIT = TK_C_OPTR_NEG + 1,

    TK_C_OPTR_GE      = TK_C_OPTR_DUAL(TK_C_OPTR_GT, TK_C_OPTR_EQ),               // >=
    TK_C_OPTR_LE      = TK_C_OPTR_DUAL(TK_C_OPTR_LT, TK_C_OPTR_EQ),               // <=
    TK_C_OPTR_ORE     = TK_C_OPTR_DUAL(TK_C_OPTR_OR, TK_C_OPTR_OR),               // |=
    TK_C_OPTR_ADDE    = TK_C_OPTR_DUAL(TK_C_OPTR_ADD, TK_C_OPTR_EQ),              // +=
    TK_C_OPTR_SUBE    = TK_C_OPTR_DUAL(TK_C_OPTR_SUB, TK_C_OPTR_EQ),              // -=
    TK_C_OPTR_MULE    = TK_C_OPTR_DUAL(TK_C_OPTR_MUL, TK_C_OPTR_EQ),              // *=
    TK_C_OPTR_ANDE    = TK_C_OPTR_DUAL(TK_C_OPTR_AND, TK_C_OPTR_EQ),              // &=
    TK_C_OPTR_XORE    = TK_C_OPTR_DUAL(TK_C_OPTR_XOR, TK_C_OPTR_EQ),              // ^=
    TK_C_OPTR_DIVE    = TK_C_OPTR_DUAL(TK_C_OPTR_DIV, TK_C_OPTR_EQ),              // /=
    TK_C_OPTR_SHT_L   = TK_C_OPTR_DUAL(TK_C_OPTR_LT, TK_C_OPTR_LT),               // <<
    TK_C_OPTR_SHT_R   = TK_C_OPTR_DUAL(TK_C_OPTR_GT, TK_C_OPTR_GT),               // >>
    TK_C_OPTR_REF_PTR = TK_C_OPTR_DUAL(TK_C_OPTR_SUB, TK_C_OPTR_GT),              // ->
    TK_C_OPTR_SHTE_L  = TK_C_OPTR_TRIPLE(TK_C_OPTR_LT, TK_C_OPTR_LT, TK_C_OPTR_EQ), // <<=
    TK_C_OPTR_SHTE_R  = TK_C_OPTR_TRIPLE(TK_C_OPTR_GT, TK_C_OPTR_GT, TK_C_OPTR_EQ), // >>=
};

/*
 * The element sequence should be the same as token_lang_c_keywords(token_data.h).
 */
enum token_language_c_kywd_type {
    TK_C_IDTR_ASM,
    TK_C_IDTR_AUTO,
    TK_C_IDTR_BREAK,
    TK_C_IDTR_CASE,
    TK_C_IDTR_CHAR,
    TK_C_IDTR_CONST,
    TK_C_IDTR_CONTINUE,
    TK_C_IDTR_DEFAULT,
    TK_C_IDTR_DOUBLE,
    TK_C_IDTR_DO,
    TK_C_IDTR_ELSE,
    TK_C_IDTR_ENUM,
    TK_C_IDTR_EXTERN,
    TK_C_IDTR_FLOAT,
    TK_C_IDTR_FOR,
    TK_C_IDTR_GOTO,
    TK_C_IDTR_IF,
    TK_C_IDTR_INLINE,
    TK_C_IDTR_INT,
    TK_C_IDTR_LONG,
    TK_C_IDTR_REGISTER,
    TK_C_IDTR_RESTRICT,
    TK_C_IDTR_RETURN,
    TK_C_IDTR_SHORT,
    TK_C_IDTR_SIGNED,
    TK_C_IDTR_SIZEOF,
    TK_C_IDTR_STATIC,
    TK_C_IDTR_STRUCT,
    TK_C_IDTR_SWITCH,
    TK_C_IDTR_TYPEDEF,
    TK_C_IDTR_UNION,
    TK_C_IDTR_UNSIGNED,
    TK_C_IDTR_VOID,
    TK_C_IDTR_VOLATILE,
    TK_C_IDTR_WHILE,
    TK_C_IDTR_NONE,

    TK_C_IDTR_FIRST = TK_C_IDTR_ASM,
    TK_C_IDTR_LAST  = TK_C_IDTR_NONE,
};

enum token_language_c_pctt_type {
    TK_C_PCTT_COMMA     = ',',
    TK_C_PCTT_S_BRKT_L  = '(',
    TK_C_PCTT_S_BRKT_R  = ')',
    TK_C_PCTT_M_BRKT_L  = '[',
    TK_C_PCTT_M_BRKT_R  = ']',
    TK_C_PCTT_L_BRKT_L  = '{',
    TK_C_PCTT_L_BRKT_R  = '}',
    TK_C_PCTT_SEMICOLON = ';',
};

/* only used for tokenizer language for keyword trie */
struct token_language_c_kywd {
    char                           c;
    e_token_language_c_kywd_type_t type;
    s_token_language_c_kywd_t      *children[TK_KYWD_CHILD_MAX];
};

struct token_language_c_optr {
    e_token_language_c_optr_type_t type;
};

struct token_language_c_idtr {
    bool is_keyword;
    union {
        e_token_language_c_kywd_type_t type;
        char                           *name;
    };
};

struct token_language_c_cnst {
    char *name;
};

struct token_language_c_pctt {
    e_token_language_c_pctt_type_t type;
};

#endif

