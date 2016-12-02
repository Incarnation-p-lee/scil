static inline void
regular_polish_optr_bracket_eliminate(s_array_stack_t *stack_opt,
    s_array_stack_t *stack_data)
{
    char *c;

    assert_exit(stack_opt && stack_data);

    c = array_stack_top(stack_opt);

    while (!regular_char_bracket_left_p(*c)) {
        c = array_stack_pop(stack_opt);
        array_stack_push(stack_data, c);
        c = array_stack_top(stack_opt);
    }

    array_stack_pop(stack_opt);
}

static inline void
regular_polish_optr_p0_process(s_array_stack_t *stack, char *c)
{
    assert_exit(c);
    assert_exit(stack);
    assert_exit(regular_stack_top_p(stack, RE_WILD_BKT_L));

    switch (*c) {
        case RE_WILD_AND:
        case RE_WILD_OR:
        case RE_WILD_STAR:
        case RE_WILD_PLUS:
        case RE_WILD_QUST:
            array_stack_push(stack, c);
            break;
        case RE_WILD_BKT_R:
            array_stack_pop(stack);
            break;
        default:
            assert_exit(false);
            break;
    }
}

static inline void
regular_polish_optr_p1_process(s_array_stack_t *stack_opt,
    s_array_stack_t *stack_data, char *c)
{
    assert_exit(c && stack_opt && stack_data);
    assert_exit(regular_stack_top_wildcard_unary_p(stack_opt));

    switch (*c) {
        case RE_WILD_OR:
        case RE_WILD_AND:
        case RE_WILD_STAR:
        case RE_WILD_PLUS:
        case RE_WILD_QUST:
            array_stack_push(stack_data, array_stack_pop(stack_opt));
            array_stack_push(stack_opt, c);
            break;
        case RE_WILD_BKT_R:
            regular_polish_optr_bracket_eliminate(stack_opt, stack_data);
            break;
        default:
            assert_exit(false);
            break;
    }
}

static inline void
regular_polish_optr_p2_process(s_array_stack_t *stack_opt,
    s_array_stack_t *stack_data, char *c)
{
    assert_exit(c && stack_opt && stack_data);
    assert_exit(regular_stack_top_p(stack_opt, RE_WILD_AND));

    switch (*c) {
        case RE_WILD_STAR:
        case RE_WILD_PLUS:
        case RE_WILD_QUST:
            array_stack_push(stack_opt, c);
            break;
        case RE_WILD_OR:
        case RE_WILD_AND:
            array_stack_push(stack_data, array_stack_pop(stack_opt));
            array_stack_push(stack_opt, c);
            break;
        case RE_WILD_BKT_R:
            regular_polish_optr_bracket_eliminate(stack_opt, stack_data);
            break;
        default:
            assert_exit(false);
            break;
    }
}

static inline void
regular_polish_optr_p3_process(s_array_stack_t *stack_opt,
    s_array_stack_t *stack_data, char *c)
{
    assert_exit(c && stack_opt && stack_data);
    assert_exit(regular_stack_top_p(stack_opt, RE_WILD_OR));

    switch (*c) {
        case RE_WILD_STAR:
        case RE_WILD_PLUS:
        case RE_WILD_QUST:
        case RE_WILD_AND:
            array_stack_push(stack_opt, c);
            break;
        case RE_WILD_OR:
            array_stack_push(stack_data, array_stack_pop(stack_opt));
            array_stack_push(stack_opt, c);
            break;
        case RE_WILD_BKT_R:
            regular_polish_optr_bracket_eliminate(stack_opt, stack_data);
            break;
        default:
            assert_exit(false);
            break;
    }
}

static inline void
regular_polish_optr_process(s_array_stack_t *stack_data,
    s_array_stack_t *stack_opt, char *c)
{
    char *top;

    assert_exit(c);
    assert_exit(stack_opt);
    assert_exit(stack_data);
    assert_exit(regular_char_wildcard_p(*c));
    assert_exit(!array_stack_empty_p(stack_opt));

    top = array_stack_top(stack_opt);

    switch (*top) {
        case RE_WILD_BKT_L:
            regular_polish_optr_p0_process(stack_opt, c);
            break;
        case RE_WILD_STAR:
        case RE_WILD_PLUS:
        case RE_WILD_QUST:
            regular_polish_optr_p1_process(stack_opt, stack_data, c);
            break;
        case RE_WILD_AND:
            regular_polish_optr_p2_process(stack_opt, stack_data, c);
            break;
        case RE_WILD_OR:
            regular_polish_optr_p3_process(stack_opt, stack_data, c);
            break;
        default:
            assert_exit(false);
            break;
    }
}

static inline char *
regular_polish_process_final(s_array_stack_t *stack, uint32 size)
{
    uint32 stack_size;
    char *tmp, *polish;

    assert_exit(stack);
    assert_exit(size > array_stack_size(stack));

    stack_size = array_stack_size(stack);
    polish = dp_malloc(sizeof(char) * size);

    polish[stack_size] = NULL_CHAR;
    do {
        tmp = array_stack_pop(stack);
        polish[--stack_size] = *tmp;
    } while (0 != stack_size);

    assert_exit(regular_polish_legal_p(polish));
    return polish;
}

static inline char *
regular_polish_process_i(char *normal)
{
    uint32 size;
    char *c, *polish;
    s_array_stack_t *stack_opt, *stack_data;

    assert_exit(normal);

    stack_opt = array_stack_create();
    stack_data = array_stack_create();

    c = normal;
    while (*c) {
        if (regular_char_data_p(*c)) {
            array_stack_push(stack_data, c);
        } else if (array_stack_empty_p(stack_opt)) {
            array_stack_push(stack_opt, c);
        } else if (regular_char_bracket_left_p(*c)) {
            array_stack_push(stack_opt, c);
        } else {
            regular_polish_optr_process(stack_data, stack_opt, c);
        }
        c++;
    }

    while (!array_stack_empty_p(stack_opt)) {
        array_stack_push(stack_data, array_stack_pop(stack_opt));
    }

    size = strlen(normal) + 1;
    polish = regular_polish_process_final(stack_data, size);

    array_stack_destroy(&stack_opt);
    array_stack_destroy(&stack_data);

    return polish;
}

/*
 * Convert Regular Expression => Reverse Polish Expression
 */
char *
regular_polish_process(char *re)
{
    char *normal, *polish;

    if (!re) {
        return PTR_INVALID;
    } else {
        normal = regular_normalize(re);
        polish = regular_polish_process_i(normal);

        dp_free(normal);
        REGULAR_CVT_TO_POLISH_PRINT(polish);

        return polish;
    }
}

