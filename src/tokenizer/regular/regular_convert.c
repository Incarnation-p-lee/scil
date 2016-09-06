bool
regular_char_translated_p(char c)
{
    if (TRANS_MASK == (c & TRANS_MASK)) {
        return true;
    } else {
        return false;
    }
}

char
regular_char_translate_resume(char c)
{
    assert_exit(regular_char_translated_p(c));
    assert_exit(regular_char_meta_p(c & TRANS_UNMASK));

    return c & TRANS_UNMASK;
}

bool
regular_char_meta_p(char c)
{
    if (RE_M_OPT_BKT_L == c || RE_M_OPT_BKT_R == c) {
        return true;
    } else if (RE_M_OPT_MBKT_L == c || RE_M_OPT_MBKT_R == c) {
        return true;
    } else if (RE_M_OPT_AND == c || RE_M_OPT_OR == c) {
        return true;
    } else if (RE_M_OPT_STAR == c || RE_M_OPT_PLUS == c || RE_M_OPT_QUST == c) {
        return true;
    } else if (RE_M_OPT_CNNT ==c) {
        return true;
    } else {
        return false;
    }
}

bool
regular_char_data_p(char c)
{
    if (dp_isalpha(c) || dp_isdigit(c) || '_' == c) {
        return true;
    } else if (regular_char_translated_p(c)) {
        return true;
    } else {
        return false;
    }
}

bool
regular_opt_bracket_left_p(char c)
{
    if (RE_M_OPT_BKT_L == c) {
        return true;
    } else {
        return false;
    }
}

bool
regular_opt_bracket_right_p(char c)
{
    if (RE_M_OPT_BKT_R == c) {
        return true;
    } else {
        return false;
    }
}

bool
regular_opt_bracket_p(char c)
{
    if (c == RE_M_OPT_BKT_R || c == RE_M_OPT_BKT_L) {
        return true;
    } else {
        return false;
    }
}

bool
regular_opt_unary_p(char c)
{
    if (RE_M_OPT_STAR == c || RE_M_OPT_PLUS == c || RE_M_OPT_QUST == c) {
        return true;
    } else {
        return false;
    }
}

bool
regular_opt_binary_p(char c)
{
    if (RE_M_OPT_OR == c || RE_M_OPT_AND == c) {
        return true;
    } else {
        return false;
    }
}

bool
regular_opt_p(char c)
{
    if (regular_opt_unary_p(c)) {
        return true;
    } else if (regular_opt_binary_p(c)) {
        return true;
    } else if (regular_opt_bracket_p(c)) {
        return true;
    } else {
        return false;
    }
}

static inline void
regular_convert_to_reverse_polish_priority_0(s_array_stack_t *stack, char *c)
{
    assert_exit(c);
    assert_exit(stack);
    assert_exit(regular_stack_top_p(stack, RE_M_OPT_BKT_L));

    switch (*c) {
        case RE_M_OPT_AND:
        case RE_M_OPT_OR:
        case RE_M_OPT_STAR:
        case RE_M_OPT_PLUS:
        case RE_M_OPT_QUST:
            array_stack_push(stack, c);
            break;
        case RE_M_OPT_BKT_R:
            array_stack_pop(stack);
            break;
        default:
            assert_exit(false);
            break;
    }
}

static inline void
regular_convert_to_reverse_polish_priority_1(s_array_stack_t *stack_opt,
    s_array_stack_t *stack_data, char *c)
{
    char *tmp;

    assert_exit(c && stack_opt && stack_data);
    assert_exit(regular_stack_top_p(stack_opt, RE_M_OPT_STAR)
        || regular_stack_top_p(stack_opt, RE_M_OPT_PLUS)
        || regular_stack_top_p(stack_opt, RE_M_OPT_QUST));

    switch (*c) {
        case RE_M_OPT_OR:
        case RE_M_OPT_AND:
        case RE_M_OPT_STAR:
        case RE_M_OPT_PLUS:
        case RE_M_OPT_QUST:
            array_stack_push(stack_data, array_stack_pop(stack_opt));
            array_stack_push(stack_opt, c);
            break;
        case RE_M_OPT_BKT_R:
            tmp = array_stack_top(stack_opt);
	    while (!regular_opt_bracket_left_p(*tmp)) {
                array_stack_push(stack_data, array_stack_pop(stack_opt));
                tmp = array_stack_top(stack_opt);
            }
            array_stack_pop(stack_opt);
            break;
        default:
            assert_exit(false);
            break;
    }
}

static inline void
regular_convert_to_reverse_polish_priority_2(s_array_stack_t *stack_opt,
    s_array_stack_t *stack_data, char *c)
{
    char *tmp;

    assert_exit(c && stack_opt && stack_data);
    assert_exit(regular_stack_top_p(stack_opt, RE_M_OPT_AND));

    switch (*c) {
        case RE_M_OPT_STAR:
        case RE_M_OPT_PLUS:
        case RE_M_OPT_QUST:
            array_stack_push(stack_opt, c);
            break;
        case RE_M_OPT_OR:
        case RE_M_OPT_AND:
            array_stack_push(stack_data, array_stack_pop(stack_opt));
            array_stack_push(stack_opt, c);
            break;
        case RE_M_OPT_BKT_R:
            tmp = array_stack_top(stack_opt);
	    while (!regular_opt_bracket_left_p(*tmp)) {
                array_stack_push(stack_data, array_stack_pop(stack_opt));
                tmp = array_stack_top(stack_opt);
            }
            array_stack_pop(stack_opt);
            break;
        default:
            assert_exit(false);
            break;
    }
}

static inline void
regular_convert_to_reverse_polish_priority_3(s_array_stack_t *stack_opt,
    s_array_stack_t *stack_data, char *c)
{
    char *tmp;
    assert_exit(c && stack_opt && stack_data);
    assert_exit(regular_stack_top_p(stack_opt, RE_M_OPT_OR));

    switch (*c) {
        case RE_M_OPT_STAR:
        case RE_M_OPT_PLUS:
        case RE_M_OPT_QUST:
        case RE_M_OPT_AND:
            array_stack_push(stack_opt, c);
            break;
        case RE_M_OPT_OR:
            array_stack_push(stack_data, array_stack_pop(stack_opt));
            array_stack_push(stack_opt, c);
            break;
        case RE_M_OPT_BKT_R:
            tmp = array_stack_top(stack_opt);
	    while (!regular_opt_bracket_left_p(*tmp)) {
                array_stack_push(stack_data, array_stack_pop(stack_opt));
                tmp = array_stack_top(stack_opt);
            }
            array_stack_pop(stack_opt);
            break;
        default:
            assert_exit(false);
            break;
    }
}

static inline void
regular_convert_to_reverse_polish_opt(s_array_stack_t *stack_data,
    s_array_stack_t *stack_opt, char *c)
{
    char *top;

    assert_exit(c);
    assert_exit(stack_opt);
    assert_exit(stack_data);
    assert_exit(regular_opt_p(*c));
    assert_exit(!array_stack_empty_p(stack_opt));

    top = array_stack_top(stack_opt);

    switch (*top) {
        case RE_M_OPT_BKT_L:
            regular_convert_to_reverse_polish_priority_0(stack_opt, c);
            break;
        case RE_M_OPT_STAR:
        case RE_M_OPT_PLUS:
        case RE_M_OPT_QUST:
            regular_convert_to_reverse_polish_priority_1(stack_opt, stack_data, c);
            break;
        case RE_M_OPT_AND:
            regular_convert_to_reverse_polish_priority_2(stack_opt, stack_data, c);
            break;
        case RE_M_OPT_OR:
            regular_convert_to_reverse_polish_priority_3(stack_opt, stack_data, c);
            break;
        default:
            assert_exit(false);
            break;
    }
}

static inline char *
regular_convert_to_reverse_polish_final(s_array_stack_t *stack, uint32 size)
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

    assert_exit(regular_reverse_polish_legal_p(polish));
    return polish;
}

/*
 * Convert Regular Expression => Reverse Polish Expression
 */
char *
regular_convert_to_reverse_polish(char *re)
{
    uint32 size;
    char *c, *normal, *polish;
    s_array_stack_t *stack_opt, *stack_data;

    assert_exit(re);

    normal = regular_normalize(re);
    stack_opt = array_stack_create();
    stack_data = array_stack_create();

    c = normal;
    while (*c) {
        if (regular_char_data_p(*c)) {
            array_stack_push(stack_data, c);
        } else if (regular_char_translated_p(*c)) {
            array_stack_push(stack_data, c++);
            array_stack_push(stack_data, c);
        } else if (array_stack_empty_p(stack_opt) || RE_M_OPT_BKT_L == *c) {
            array_stack_push(stack_opt, c);
        } else {
            regular_convert_to_reverse_polish_opt(stack_data, stack_opt, c);
        }
        c++;
    }

    while (!array_stack_empty_p(stack_opt)) {
        array_stack_push(stack_data, array_stack_pop(stack_opt));
    }

    size = dp_strlen(normal) + 1;
    polish = regular_convert_to_reverse_polish_final(stack_data, size);

    dp_free(normal);
    array_stack_destroy(&stack_opt);
    array_stack_destroy(&stack_data);

    REGULAR_CVT_TO_REVERSE_POLISH_PRINT(polish);
    return polish;
}

