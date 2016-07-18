static inline bool
nfa_engine_stack_opt_top_p(s_array_stack_t *stack, char expected)
{
    char *tmp;

    assert_exit(stack);

    tmp = array_stack_top(stack);

    return *tmp == expected;
}

