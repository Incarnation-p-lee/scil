static inline bool
nfa_engine_stack_opt_top_p(s_array_stack_t *stack, char expected)
{
    char *tmp;

    assert_exit(stack);

    tmp = array_stack_top(stack);

    return *tmp == expected;
}

static inline bool
nfa_engine_reverse_polish_legal_p(char *rp)
{
    char *c;
    char tmp;
    bool is_legal;
    s_array_stack_t *stack;

    assert_exit(rp);

    c = rp;
    tmp = NULL_CHAR;
    stack = array_stack_create();

    while (*c) {
        if (nfa_char_alnum_underline_p(*c)) {
            array_stack_push(stack, &tmp);
        } else if (nfa_char_binary_opt_p(*c)) {
            array_stack_pop(stack);
            array_stack_pop(stack);
            array_stack_push(stack, &tmp);
        } else if (nfa_char_unary_opt_p(*c)) {
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

