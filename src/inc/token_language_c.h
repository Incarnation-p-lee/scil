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
#define LANGUAGE_C_RE_OPTR           "`+|`*|`?|-|/|`||~|^|%|!|`&|.|:|=|>|<"
#define LANGUAGE_C_RE_CNST           "[0-9]+|\"[a-zA-Z0-9_ #%]+\""
#define LANGUAGE_C_RE_PCTT           "`[`],;`(`){}"

/* For Language C, 16 children size of keyword trie is enough */
#define TK_KYWD_CHILD_MAX            16
#define TK_LANG_UNMATCH              0

typedef enum token_language_c_optr_type e_token_language_c_optr_type_t;
typedef enum token_language_c_kywd_type e_token_language_c_kywd_type_t;
typedef enum token_language_c_pctt_type e_token_language_c_pctt_type_t;

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

    TK_C_OPTR_GE      = TK_2_CHAR_JOIN(TK_C_OPTR_GT, TK_C_OPTR_EQ),               // >=
    TK_C_OPTR_LE      = TK_2_CHAR_JOIN(TK_C_OPTR_LT, TK_C_OPTR_EQ),               // <=
    TK_C_OPTR_ORE     = TK_2_CHAR_JOIN(TK_C_OPTR_OR, TK_C_OPTR_OR),               // |=
    TK_C_OPTR_ADDE    = TK_2_CHAR_JOIN(TK_C_OPTR_ADD, TK_C_OPTR_EQ),              // +=
    TK_C_OPTR_SUBE    = TK_2_CHAR_JOIN(TK_C_OPTR_SUB, TK_C_OPTR_EQ),              // -=
    TK_C_OPTR_MULE    = TK_2_CHAR_JOIN(TK_C_OPTR_MUL, TK_C_OPTR_EQ),              // *=
    TK_C_OPTR_ANDE    = TK_2_CHAR_JOIN(TK_C_OPTR_AND, TK_C_OPTR_EQ),              // &=
    TK_C_OPTR_XORE    = TK_2_CHAR_JOIN(TK_C_OPTR_XOR, TK_C_OPTR_EQ),              // ^=
    TK_C_OPTR_DIVE    = TK_2_CHAR_JOIN(TK_C_OPTR_DIV, TK_C_OPTR_EQ),              // /=
    TK_C_OPTR_SHT_L   = TK_2_CHAR_JOIN(TK_C_OPTR_LT, TK_C_OPTR_LT),               // <<
    TK_C_OPTR_SHT_R   = TK_2_CHAR_JOIN(TK_C_OPTR_GT, TK_C_OPTR_GT),               // >>
    TK_C_OPTR_REF_PTR = TK_2_CHAR_JOIN(TK_C_OPTR_SUB, TK_C_OPTR_GT),              // ->
    TK_C_OPTR_SHTE_L  = TK_3_CHAR_JOIN(TK_C_OPTR_LT, TK_C_OPTR_LT, TK_C_OPTR_EQ), // <<=
    TK_C_OPTR_SHTE_R  = TK_3_CHAR_JOIN(TK_C_OPTR_GT, TK_C_OPTR_GT, TK_C_OPTR_EQ), // >>=
};

/*
 * The element sequence should be the same as token_lang_c_keywords(token_data.h).
 */
enum token_language_c_kywd_type { /* placehoulder '-' */
    TK_C_KYWD_NONE,
    TK_C_KYWD_ASM      = TK_4_CHAR_JOIN('a', 's', 'm', '-'),
    TK_C_KYWD_AUTO     = TK_4_CHAR_JOIN('a', 'u', 't', 'o'),
    TK_C_KYWD_BREAK    = TK_4_CHAR_JOIN('b', 'r', 'e', 'a'),
    TK_C_KYWD_CASE     = TK_4_CHAR_JOIN('c', 'a', 's', 'e'),
    TK_C_KYWD_CHAR     = TK_4_CHAR_JOIN('c', 'h', 'a', 'r'),
    TK_C_KYWD_CONST    = TK_4_CHAR_JOIN('c', 'o', 'n', 's'),
    TK_C_KYWD_CONTINUE = TK_4_CHAR_JOIN('c', 'o', 'n', 't'),
    TK_C_KYWD_DEFAULT  = TK_4_CHAR_JOIN('d', 'e', 'f', 'a'),
    TK_C_KYWD_DOUBLE   = TK_4_CHAR_JOIN('d', 'o', 'u', 'b'),
    TK_C_KYWD_DO       = TK_4_CHAR_JOIN('d', 'o', '-', '-'),
    TK_C_KYWD_ELSE     = TK_4_CHAR_JOIN('e', 'l', 's', 'e'),
    TK_C_KYWD_ENUM     = TK_4_CHAR_JOIN('e', 'n', 'u', 'm'),
    TK_C_KYWD_EXTERN   = TK_4_CHAR_JOIN('e', 'x', 't', 'e'),
    TK_C_KYWD_FLOAT    = TK_4_CHAR_JOIN('f', 'l', 'o', 'a'),
    TK_C_KYWD_FOR      = TK_4_CHAR_JOIN('f', 'o', 'r', '-'),
    TK_C_KYWD_GOTO     = TK_4_CHAR_JOIN('g', 'o', 't', 'o'),
    TK_C_KYWD_IF       = TK_4_CHAR_JOIN('i', 'f', '-', '-'),
    TK_C_KYWD_INLINE   = TK_4_CHAR_JOIN('i', 'n', 'l', 'i'),
    TK_C_KYWD_INT      = TK_4_CHAR_JOIN('i', 'n', 't', '-'),
    TK_C_KYWD_LONG     = TK_4_CHAR_JOIN('l', 'o', 'n', 'g'),
    TK_C_KYWD_REGISTER = TK_4_CHAR_JOIN('r', 'e', 'g', 'i'),
    TK_C_KYWD_RESTRICT = TK_4_CHAR_JOIN('r', 'e', 's', 't'),
    TK_C_KYWD_RETURN   = TK_4_CHAR_JOIN('r', 'e', 't', 'u'),
    TK_C_KYWD_SHORT    = TK_4_CHAR_JOIN('s', 'h', 'o', 'r'),
    TK_C_KYWD_SIGNED   = TK_4_CHAR_JOIN('s', 'i', 'g', 'n'),
    TK_C_KYWD_SIZEOF   = TK_4_CHAR_JOIN('s', 'i', 'z', 'e'),
    TK_C_KYWD_STATIC   = TK_4_CHAR_JOIN('s', 't', 'a', 't'),
    TK_C_KYWD_STRUCT   = TK_4_CHAR_JOIN('s', 't', 'r', 'u'),
    TK_C_KYWD_SWITCH   = TK_4_CHAR_JOIN('s', 'w', 'i', 't'),
    TK_C_KYWD_TYPEDEF  = TK_4_CHAR_JOIN('t', 'y', 'p', 'e'),
    TK_C_KYWD_UNIONEF  = TK_4_CHAR_JOIN('u', 'n', 'i', 'o'),
    TK_C_KYWD_UNSIGNED = TK_4_CHAR_JOIN('u', 'n', 's', 'i'),
    TK_C_KYWD_VOID     = TK_4_CHAR_JOIN('v', 'o', 'i', 'd'),
    TK_C_KYWD_VOLATILE = TK_4_CHAR_JOIN('v', 'o', 'l', 'a'),
    TK_C_KYWD_WHILE    = TK_4_CHAR_JOIN('w', 'h', 'i', 'l'),
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

