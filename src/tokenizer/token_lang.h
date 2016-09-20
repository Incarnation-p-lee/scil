#ifndef HAVE_DEFINED LANG_C
#define HAVE_DEFINED LANG_C

#define TOKEN_LOW_LTR_SIZE          26
#define TOKEN_LANG_SUB_SIZE         16

typedef struct token_lang_c_keyword s_token_lang_c_keyword_t;

struct token_lang_c_keyword {
    char                          c;
    e_token_lang_c_keyword_type_t type;
    s_token_lang_c_keyword_t      *children[TOKEN_LANG_SUB_SIZE];
};

#endif

