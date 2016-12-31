static inline e_gr_non_tr_type_t
grammar_string_non_terminal_obtain(char *name)
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

    return GR_NON_TR_INVALID;
}

static inline void
grammar_string_head_obtain(char *symbol, uint32 size, char *pdt)
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
        } else if (c[0] == GR_STR_INFER_HEAD) {
            if (c[1] == GR_STR_INFER_TAIL) {
                break;
            }

            log_print_and_exit("Invalid format of production '%s'\n", pdt);
        }

        c++;
    }

    symbol[i] = NULL_CHAR;
}

static inline uint32
grammar_string_body_fill(char *body, uint32 size, char *body_list)
{
    char *c;
    uint32 i;

    assert_exit(size > 0);
    assert_exit(body && body_list);

    i = 0;
    c = body_list;
    c = string_space_skip(c);

    while (*c && *c != GR_STR_BODY_ISOLATOR) {
        body[i++] = *c++;

        if (i >= size - 1) { /* reserved 1 char for NULL_CHAR */
            log_print_and_exit("Grammar body overflow.\n");
        }
    }

    c = string_space_skip(c);
    body[i] = NULL_CHAR;

    return (uint32)(c - body_list);
}

static inline uint32
grammar_string_symbol_fill(char *symbol, uint32 size, char *body)
{
    char *c;
    uint32 i;

    assert_exit(size > 0);
    assert_exit(symbol && body);

    i = 0;
    c = body;
    c = string_space_skip(c);

    while (*c && *c != GR_STR_SYMBOL_ISOLATOR) {
        symbol[i++] = *c++;

        if (i >= size - 1) { /* reserved 1 char for NULL_CHAR */
            log_print_and_exit("Grammar symbol overflow.\n");
        }
    }

    c = string_space_skip(c);
    symbol[i] = NULL_CHAR;

    return (uint32)(c - body);
}

