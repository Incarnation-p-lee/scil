static inline e_gr_non_tr_type_t
gr_string_non_terminal_obtain(char *name)
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

static inline e_gr_tr_type_t
gr_string_terminal_obtain(char *name)
{
    uint32 i;

    assert_exit(name);

    i = 0;

    while (i < ARRAY_SIZE_OF(tr_type_name)) {
        if (dp_strcmp(name, tr_type_name[i]) == 0) {
            return (e_gr_tr_type_t)i;
        }

        i++;
    }

    return GR_TR_INVALID;
}

static inline void
gr_string_head_obtain(char *symbol, uint32 size, char *pdt)
{
    char *c;
    uint32 i;

    assert_exit(size > 0);
    assert_exit(symbol && pdt);

    i = 0;
    c = pdt;

    while (*c) {
        if (c[0] == GR_STR_INFER_HEAD) {
            if (c[1] == GR_STR_INFER_TAIL) {
                break;
            }

            log_print_and_exit("Invalid format of production '%s'\n", pdt);
        } else if (!dp_isspace(*c)) {
            symbol[i++] = *c;

            if (i >= size) {
                log_print_and_exit("Grammar symbol overflow.\n");
            }
        }
        c++;
    }

    symbol[i] = NULL_CHAR;
}

static inline char *
gr_string_head_skip(char *pdt)
{
    char *c;

    assert_exit(pdt);

    c = pdt;

    while (*c) {
        if (c[0] == GR_STR_INFER_HEAD && c[1] == GR_STR_INFER_TAIL) {
            return string_space_skip(c + 2);
        }
        c++;
    }

    log_print_and_exit("Unknown production detected.\n");

    return PTR_INVALID;
}

static inline uint32
gr_string_body_fill(char *body, uint32 size, char *body_list)
{
    char *c;
    uint32 i;

    assert_exit(size > 0);
    assert_exit(body && body_list);

    i = 0;
    c = body_list;
    c = string_space_skip(c);

    while (*c) {
        if (*c == GR_STR_BODY_ISOLATOR) {
            c++; /* skip the body isolator */
            break;
        }

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
gr_string_symbol_fill(char *symbol, uint32 size, char *body)
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

