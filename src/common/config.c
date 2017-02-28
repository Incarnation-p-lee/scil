static inline uint32
config_option_grammar_type(char *assign)
{
    char *c;
    uint32 i;

    assert_exit(assign);

    i = 0;
    c = string_space_skip(assign);

    while (!dp_isspace(c[i])) {
        i++;
    }

    c[i] = NULL_CHAR;

    if (dp_strcmp(c, GR_TYPE_NAME_LL_1) == 0) {
        return GR_TYPE_LL_1;
    } else {
        return GR_TYPE_UNKNOWN;
    }
}

static inline void
config_option_type_set(char *option, char *assign)
{
    char *c;
    uint32 i;

    assert_exit(option && assign);
    assert_exit(assign == dp_strchr(option, SCIL_CONFIG_ASSIGN));

    i = 0;
    c = option;

    while (!dp_isspace(c[i]) && c[i] != SCIL_CONFIG_ASSIGN) {
        i++;
    }

    c[i] = NULL_CHAR;
    assign++;

    if (dp_strcmp(c, GR_TYPE) == 0) {
        config_list.grammar_type = config_option_grammar_type(assign);
    }
}

static inline void
config_option_bool_set(char *option)
{
    char *c;
    uint32 i;

    assert_exit(option);

    i = 0;
    c = option;

    while (!dp_isspace(c[i])) {
        i++;
    }

    c[i] = NULL_CHAR;

    if (dp_strcmp(c, NFA_VERBOSE) == 0) {
        config_list.nfa_verbose = true;
    } else if (dp_strcmp(c, RE_VERBOSE) == 0) {
        config_list.regular_verbose = true;
    } else if (dp_strcmp(c, TK_VERBOSE) == 0) {
        config_list.token_verbose = true;
    } else if (dp_strcmp(c, TKZ_VERBOSE) == 0) {
        config_list.tokenizer_verbose = true;
    } else if (dp_strcmp(c, GR_VERBOSE) == 0) {
        config_list.grammar_verbose = true;
    } else {
        log_print("Unknown bool config '%s'\n", c);
    }
}

static inline void
config_option_line_parse(char *option)
{
    char *c;
    char *assign;

    assert_exit(option);

    c = string_space_skip(option);

    if (*c == SCIL_CONFIG_COMMENT) {
        return;
    } else {
        c = string_space_skip(c);
        assign = dp_strchr(option, SCIL_CONFIG_ASSIGN);

        if (assign == NULL) {
            config_option_bool_set(c);
        } else {
            config_option_type_set(c, assign);
        }
    }
}

static inline bool
config_option_line_empty_p(char *config_buf)
{
    char *c;

    assert_exit(config_buf);

    c = config_buf;

    while (*c == SCIL_CONFIG_SPACE || *c == SCIL_CONFIG_COMMENT) {
        c++;
    }

    if (*c == SCIL_CONFIG_NEWLINE) {
        return true;
    } else {
        return false;
    }
}

static inline void
config_option_parse(FILE *config)
{
    assert_exit(config);

    while (dp_fgets(config_buf, SCIL_CONFIG_LINE_MAX, config) == config_buf) {
        if (config_option_line_empty_p(config_buf)) {
            continue;
        }
        config_option_line_parse(config_buf);
    }
}

void
config_initial(void)
{
    FILE *config_file;

    config_file = dp_fopen(SCIL_CONFIG_FILE, "r");

    if (!config_file) {
        dp_printf("Failed to open config %s.\n", SCIL_CONFIG_FILE);
    } else {
        log_print("[LOG] Read config file %s done.\n", SCIL_CONFIG_FILE);
        config_option_parse(config_file);
    }

    dp_fclose(config_file);
}

bool
config_nfa_verbose_p(void)
{
    return config_list.nfa_verbose;
}

bool
config_regular_verbose_p(void)
{
    return config_list.regular_verbose;
}

bool
config_tokenizer_verbose_p(void)
{
    return config_list.tokenizer_verbose;
}

bool
config_token_verbose_p(void)
{
    return config_list.token_verbose;
}

bool
config_grammar_verbose_p(void)
{
    return config_list.grammar_verbose;
}

uint32
config_grammar_type(void)
{
    return config_list.grammar_type;
}

