#ifndef HAVE_DEFINED_CONFIG_H
#define HAVE_DEFINED_CONFIG_H

#define SCIL_CONFIG_FILE       "scil.config"
#define SCIL_CONFIG_LINE_MAX   64u
#define SCIL_CONFIG_COMMENT    '#'
#define SCIL_CONFIG_ASSIGN     '='
#define SCIL_CONFIG_SPACE      ' '
#define SCIL_CONFIG_NEWLINE    '\n'

typedef struct config_list s_config_list_t;

struct config_list {
    /* verbose */
    bool nfa_verbose;
    bool regular_verbose;
    bool token_verbose;
    bool tokenizer_verbose;

    /* feature */
    uint32 grammar_type;
};

static char config_buf[SCIL_CONFIG_LINE_MAX];
static s_config_list_t config_list = {
    false,
    false,
    false,
    false,
    GRAMMAR_TYPE_LL_1,
};


#endif

