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
regular_reverse_polish_legal_p(char *polish)
{
    char *c;
    char tmp;
    bool is_legal;
    s_array_stack_t *stack;

    assert_exit(polish);

    c = polish;
    tmp = NULL_CHAR;
    stack = array_stack_create();

    while (*c) {
        if (regular_char_data_p(*c)) {
            array_stack_push(stack, &tmp);
        } else if (regular_char_translated_p(*c)) {
            assert_exit(regular_char_meta_p(c[1]));
            array_stack_push(stack, &tmp);
            c++;
        } else if (regular_opt_binary_p(*c)) {
            array_stack_pop(stack);
            array_stack_pop(stack);
            array_stack_push(stack, &tmp);
        } else if (regular_opt_unary_p(*c)) {
            array_stack_pop(stack);
            array_stack_push(stack, &tmp);
        } else {
            assert_exit(false);
        }
        c++;
    }

    array_stack_pop(stack);
    if (array_stack_empty_p(stack)) {
        is_legal = true;
    } else {
        is_legal = false;
    }

    array_stack_destroy(&stack);
    return is_legal;
}

static inline void
regular_range_expand_print(char *regular)
{
    char *c, tmp;

    assert_exit(regular);

    scil_log_print("<After regular expression range expand>\n    '");

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
regular_opt_and_insert_print(char *regular)
{
    char *c, tmp;

    assert_exit(regular);

    scil_log_print("<After regular expression opt and insert>\n    '");

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
regular_convert_to_reverse_polish_print(char *regular)
{
    char *c, tmp;

    assert_exit(regular);

    scil_log_print("<After regular expression convert to reverse polish>\n    '");

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

