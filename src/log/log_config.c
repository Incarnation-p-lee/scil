static inline void
log_config_module_option_set_tail(char *option)
{
    char *c;

    assert_exit(option);

    c = option;

    while (!dp_isspace(*c)) {
        c++;
    }

    *c = NULL_CHAR;
}

static inline char *
log_config_module_option_space_skip(char *option)
{
    char *c;

    assert_exit(option);

    c = option;

    while (dp_isspace(*c)) {
        c++;
    }

    return c;
}

static inline void
log_config_module_option_set(char *option)
{
    assert_exit(option);

    option = log_config_module_option_space_skip(option);
    RETURN_IF_EQ(option[0], SCIL_CONFIG_COMMENT);

    option = log_config_module_option_space_skip(option);
    assert_exit(option[0] == SCIL_CONFIG_MASK);

    option++; /* skip SCIL_CONFIG_MASK */
    log_config_module_option_set_tail(option);

    if (dp_strcmp(option, NFA_VERBOSE) == 0) {
        log_option_verbose[LOG_NFA_VERBOSE] = true;
    } else if (dp_strcmp(option, TOKEN_VERBOSE) == 0) {
        log_option_verbose[LOG_TOKEN_VERBOSE] = true;
    } else if (dp_strcmp(option, REGULAR_VERBOSE) == 0) {
        log_option_verbose[LOG_REGULAR_VERBOSE] = true;
    } else if (dp_strcmp(option, TOKENIZER_VERBOSE) == 0) {
        log_option_verbose[LOG_TOKENIZER_VERBOSE] = true;
    }
}

static inline void
log_config_module_option_parse(FILE *fd)
{
    assert_exit(fd);

    RETURN_IF_NE(dp_fgets(config_buf, SCIL_CONFIG_LINE_MAX, fd), config_buf);

    while (dp_strchr(config_buf, SCIL_CONFIG_MASK) != NULL) {
        log_config_module_option_set(config_buf);

        RETURN_IF_NE(dp_fgets(config_buf, SCIL_CONFIG_LINE_MAX, fd), config_buf);
    }
}

static inline void
log_config_module_seek(FILE *fd, char *module)
{
    assert_exit(module);

    RETURN_IF_NE(dp_fgets(config_buf, SCIL_CONFIG_LINE_MAX, fd), config_buf);

    while (dp_strncmp(config_buf, module, dp_strlen(module)) != 0) {
        RETURN_IF_NE(dp_fgets(config_buf, SCIL_CONFIG_LINE_MAX, fd), config_buf);
    }

    log_config_module_option_parse(fd);
}

void
log_config_initial(char *module)
{
    FILE *config;

    if (module) {
        config = dp_fopen(SCIL_CONFIG_FILE, "r");

        if (!config) {
            dp_printf("Failed to open config %s.\n", SCIL_CONFIG_FILE);
        } else {
            log_config_module_seek(config, module);
            log_print("[LOG] Read config file %s done.\n", SCIL_CONFIG_FILE);
        }
    }
}

