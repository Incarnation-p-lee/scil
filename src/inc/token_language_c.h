#ifndef HAVE_DEFINED_TOKEN_LANG_C_H
#define HAVE_DEFINED_TOKEN_LANG_C_H

#define LANG_C_RE_IDTR \
    "[A-Za-z_][a-zA-Z0-9_]*"
#define LANG_C_RE_OPTR \
    "((`+|-|`*|/|>|<|%|^|`&|~|`||=|>>|<<|!)=?)|(`?|.|:|->|`+`+|--)|`**"
#define LANG_C_RE_CNST \
    "([0-9]+)|(\"[a-zA-Z0-9_ #%,.\\]+\")"
#define LANG_C_RE_PCTT \
    "`[|`]|,|;|`(|`)|{|}"

typedef struct token_language_c_optr s_tk_lang_c_optr_t;
typedef struct token_language_c_idtr s_tk_lang_c_idtr_t;
typedef struct token_language_c_cnst s_tk_lang_c_cnst_t;
typedef struct token_language_c_pctt s_tk_lang_c_pctt_t;

#define TK_LANG_UNMATCH              0
#define TK_LANG_MATCH_INVALID        ((uint32)-1)

struct token_language_c_optr {
    char *name;
};

struct token_language_c_idtr {
    bool is_keyword;
    char *name;
};

struct token_language_c_cnst {
    char *name;
};

struct token_language_c_pctt {
    char c;
};

#endif

