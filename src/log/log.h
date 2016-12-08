#ifndef HAVE_DEFINED_LOG_H
#define HAVE_DEFINED_LOG_H

#define SCIL_CONFIG_FILE       "scil.config"
#define SCIL_CONFIG_LINE_MAX   64u
#define SCIL_CONFIG_MASK       '-'
#define SCIL_CONFIG_COMMENT    '#'

#define NFA_VERBOSE            "nfa_verbose"
#define TOKEN_VERBOSE          "token_verbose"
#define REGULAR_VERBOSE        "regular_verbose"
#define TOKENIZER_VERBOSE      "tokenizer_verbose"

enum log_verbose {
    LOG_NFA_VERBOSE,
    LOG_TOKEN_VERBOSE,
    LOG_REGULAR_VERBOSE,
    LOG_TOKENIZER_VERBOSE,
    LOG_VERBOSE_CNT,
};

typedef enum log_verbose e_log_verbose_t;

static FILE *log_file = NULL;

static char config_buf[SCIL_CONFIG_LINE_MAX];

static bool log_option_verbose[LOG_VERBOSE_CNT] = {
    false, /* nfa */
    false, /* token */
    false, /* regular */
    false, /* tokenizer */
};

#endif

