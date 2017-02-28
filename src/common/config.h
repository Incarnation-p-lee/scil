#ifndef HAVE_DEFINED_CONFIG_H
#define HAVE_DEFINED_CONFIG_H

#define SCIL_CONFIG_FILE       "scil.config"
#define SCIL_CONFIG_LINE_MAX   64u
#define SCIL_CONFIG_COMMENT    '#'
#define SCIL_CONFIG_ASSIGN     '='
#define SCIL_CONFIG_SPACE      ' '
#define SCIL_CONFIG_NEWLINE    '\n'

#define NFA_VERBOSE            "nfa_verbose"
#define TK_VERBOSE          "token_verbose"
#define RE_VERBOSE        "regular_verbose"
#define TKZ_VERBOSE      "tokenizer_verbose"
#define GR_VERBOSE             "grammar_verbose"

#define GR_TYPE                "grammar_type"
#define GR_TYPE_NAME_LL_1      "ll1"
#define GR_TYPE_NAME_LR_1      "lr1"

typedef struct config_list s_config_list_t;

struct config_list {
    /* verbose */
    bool        nfa_verbose;
    bool        regular_verbose;
    bool        token_verbose;
    bool        tokenizer_verbose;
    bool        grammar_verbose;

    /* feature */
    e_gr_type_t grammar_type;
};

static char config_buf[SCIL_CONFIG_LINE_MAX];

static s_config_list_t config_list = {
    false,
    false,
    false,
    false,
    false,
    GR_TYPE_LL_1,
};

#endif

