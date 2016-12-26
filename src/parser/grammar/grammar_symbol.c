static inline e_gr_non_tr_type_t
grammar_symbol_non_terminal_obtain(char *name)
{
    uint32 i;

    assert_exit(name);

    i = 0;

    while (i < ARRAY_SIZE_OF(non_tr_type_name)) {
        if (dp_strcmp(name, non_tr_type_name[i]) == 0) {
            return (e_gr_non_tr_type_t)i;
        }

        i++;
    }

    log_print_and_exit("Invalid non-terminal symbol '%s'.\n", name);

    return GR_NON_TR_INVALID;
}

static inline void
grammar_symbol_head_obtain(char *symbol, uint32 size, char *pdt)
{
    char *c;
    uint32 i;

    assert_exit(size > 0);
    assert_exit(symbol && pdt);

    i = 0;
    c = pdt;

    while (*c) {
        if (!dp_isspace(*c)) {
            symbol[i++] = *c;

            if (i >= size) {
                log_print_and_exit("Grammar symbol overflow.\n");
            }
        } else if (c[0] == GR_SYMBOL_INFER_HEAD) {
            if (c[1] == GR_SYMBOL_INFER_TAIL) {
                break;
            }

            log_print_and_exit("Invalid format of production '%s'\n", pdt);
        }

        c++;
    }

    symbol[i] = NULL_CHAR;
}

static inline char *
grammar_symbol_head_skip(char *pdt)
{
    char *c;

    assert_exit(pdt);

    c = pdt;
    c = dp_strchr(pdt, GR_SYMBOL_INFER_HEAD);

    if (!c && c[1] != GR_SYMBOL_INFER_TAIL) {
        log_print_and_exit("Invalid format of production '%s'\n", pdt);
    }

    return string_space_skip(c);
}

static inline uint32
grammar_symbol_body_list_count(char *body)
{
    char *c;
    uint32 cnt;

    assert_exit(body);

    cnt = 1;
    c = body;

    while (dp_strchr(body, GR_SYMBOL_BODY_ISOLATOR) != NULL) {
        cnt++;
    }

    return cnt;
}

