static inline bool
regular_stack_top_p(s_array_stack_t *stack, char expected)
{
    char *tmp;

    assert_exit(stack);

    tmp = array_stack_top(stack);
    if (*tmp == expected) {
        return true;
    } else {
        return false;
    }
}

static inline bool
regular_stack_top_wildcard_unary_p(s_array_stack_t *stack)
{
    char *unary;

    assert_exit(stack);

    unary = array_stack_top(stack);

    return regular_char_wildcard_unary_p(*unary);
}

static inline bool
regular_polish_legal_p(char *polish)
{
    char *c;
    sint32 stack;
    bool is_legal;

    assert_exit(polish);

    stack = 0;
    c = polish;

    while (*c) {
        if (regular_char_data_p(*c)) {
            stack++;
        } else if (regular_char_translated_p(*c)) {
            assert_exit(regular_char_wildcard_p(c[1]));
            c++;
            stack++;
        } else if (regular_char_wildcard_binary_p(*c)) {
            stack--;
        } else if (regular_char_wildcard_unary_p(*c)) {
            ;
        } else {
            assert_exit(false);
        }
        c++;
    }

    is_legal = stack == 1 ? true : false;

    return is_legal;
}

static inline void
regular_range_recover_print(char *regular)
{
    char *c, tmp;

    assert_exit(regular);

    scil_log_print(">> After regular expression range expand\n    '");

    c = regular;
    while (*c) {
        tmp = *c++;
        if (tmp & TRANS_MASK) {
            tmp = tmp & TRANS_UNMASK;
            scil_log_print("`");
        }
        scil_log_print("%c", tmp);
    }

    scil_log_print("'\n\n");
}

static inline void
regular_char_and_insert_print(char *regular)
{
    char *c, tmp;

    assert_exit(regular);

    scil_log_print(">> After regular expression opt and insert\n    '");

    c = regular;
    while (*c) {
        tmp = *c++;
        if (tmp & TRANS_MASK) {
            tmp = tmp & TRANS_UNMASK;
            scil_log_print("`");
        }
        scil_log_print("%c", tmp);
    }

    scil_log_print("'\n\n");
}

static inline void
regular_convert_to_polish_print(char *regular)
{
    char *c, tmp;

    assert_exit(regular);

    scil_log_print(">> After regular expression convert to polish\n    '");

    c = regular;
    while (*c) {
        tmp = *c++;
        if (tmp & TRANS_MASK) {
            tmp = tmp & TRANS_UNMASK;
            scil_log_print("`");
        }
        scil_log_print("%c", tmp);
    }

    scil_log_print("'\n\n");
}

static inline bool
regular_normalize_recover_structure_legal_p(s_regular_recover_t *recover)
{
    if (recover == NULL) {
        return false;
    } else if (recover->buf == NULL) {
        return false;
    } else if (recover->size == 0) {
        return false;
    } else if (recover->index >= recover->size) {
        return false;
    } else {
        return true;
    }
}

