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
        if (regular_data_p(*c)) {
            array_stack_push(stack, &tmp);
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

