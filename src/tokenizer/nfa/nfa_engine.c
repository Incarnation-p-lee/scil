static inline bool
nfa_engine_structure_legal_p(s_nfa_t *nfa)
{
    if (!nfa) {
        return false;
    } else if (!nfa->start || !nfa->terminal) {
        return false;
    } else if (!nfa_status_structure_legal_p(nfa->start)) {
        return false;
    } else if (!nfa_status_structure_legal_p(nfa->terminal)) {
        return false;
    } else {
        return true;
    }
}

static inline bool
nfa_engine_graph_dfs_reached_p(s_fa_status_t *start, s_fa_status_t *terminal,
    s_open_addressing_hash_t *hash)
{
    uint32 i;
    void *key;

    assert_exit(hash);
    assert_exit(start);
    assert_exit(terminal);
    assert_exit(!terminal->edge_count);

    key = (void *)(ptr_t)start->label;

    if (!open_addressing_hash_find(hash, key)) {
        open_addressing_hash_insert(&hash, key);
        if (start == terminal) {
            return true;
        }

        i = 0;
        while (i < start->edge_count) {
            assert_exit(start->edge[i]);
            if (nfa_engine_graph_dfs_reached_p(start->edge[i]->next, terminal, hash)) {
                return true;
            }
            i++;
        }
    }

    return false;
}

static inline bool
nfa_engine_graph_legal_p(s_nfa_t *nfa)
{
    bool legal;
    s_open_addressing_hash_t *hash;

    assert_exit(nfa_engine_structure_legal_p(nfa));

    hash = open_addressing_hash_create(NFA_LABEL_HASH_SIZE);
    legal = nfa_engine_graph_dfs_reached_p(nfa->start, nfa->terminal, hash);
    open_addressing_hash_destroy(&hash);

    return legal;
}

static inline void
nfa_engine_re_to_rp_top_priority_0(s_array_stack_t *stack, char *c)
{
    assert_exit(c);
    assert_exit(stack);
    assert_exit(nfa_engine_stack_top_p(stack, NFA_SUBSET_BKT_L));

    switch (*c) {
        case NFA_SUBSET_AND:
        case NFA_SUBSET_OR:
        case NFA_SUBSET_STAR:
        case NFA_SUBSET_PLUS:
        case NFA_SUBSET_QUST:
            array_stack_push(stack, c);
            break;
        case NFA_SUBSET_BKT_R:
            array_stack_pop(stack);
            break;
        default:
            assert_exit(false);
            break;
    }
}

static inline void
nfa_engine_re_to_rp_top_priority_1(s_array_stack_t *stack_opt,
    s_array_stack_t *stack_data, char *c)
{
    assert_exit(c && stack_opt && stack_data);
    assert_exit(nfa_engine_stack_top_p(stack_opt, NFA_SUBSET_STAR)
        || nfa_engine_stack_top_p(stack_opt, NFA_SUBSET_PLUS)
        || nfa_engine_stack_top_p(stack_opt, NFA_SUBSET_QUST));

    switch (*c) {
        case NFA_SUBSET_OR:
        case NFA_SUBSET_AND:
        case NFA_SUBSET_STAR:
        case NFA_SUBSET_PLUS:
        case NFA_SUBSET_QUST:
            array_stack_push(stack_data, array_stack_pop(stack_opt));
            array_stack_push(stack_opt, c);
            break;
        case NFA_SUBSET_BKT_R:
	    while (!nfa_char_bracket_left_p(array_stack_top(stack_opt))) {
                array_stack_push(stack_data, array_stack_pop(stack_opt));
            }
            break;
        default:
            assert_exit(false);
            break;
    }
}

static inline void
nfa_engine_re_to_rp_top_priority_2(s_array_stack_t *stack_opt,
    s_array_stack_t *stack_data, char *c)
{
    assert_exit(c && stack_opt && stack_data);
    assert_exit(nfa_engine_stack_top_p(stack_opt, NFA_SUBSET_AND));

    switch (*c) {
        case NFA_SUBSET_STAR:
        case NFA_SUBSET_PLUS:
        case NFA_SUBSET_QUST:
            array_stack_push(stack_opt, c);
            break;
        case NFA_SUBSET_OR:
        case NFA_SUBSET_AND:
            array_stack_push(stack_data, array_stack_pop(stack_opt));
            array_stack_push(stack_opt, c);
            break;
        case NFA_SUBSET_BKT_R:
	    while (!nfa_char_bracket_left_p(array_stack_top(stack_opt))) {
                array_stack_push(stack_data, array_stack_pop(stack_opt));
            }
            array_stack_pop(stack_opt);
            break;
        default:
            assert_exit(false);
            break;
    }
}

static inline void
nfa_engine_re_to_rp_top_priority_3(s_array_stack_t *stack_opt,
    s_array_stack_t *stack_data, char *c)
{
    assert_exit(c && stack_opt && stack_data);
    assert_exit(nfa_engine_stack_top_p(stack_opt, NFA_SUBSET_OR));

    switch (*c) {
        case NFA_SUBSET_STAR:
        case NFA_SUBSET_PLUS:
        case NFA_SUBSET_QUST:
        case NFA_SUBSET_AND:
            array_stack_push(stack_opt, c);
            break;
        case NFA_SUBSET_OR:
            array_stack_push(stack_data, array_stack_pop(stack_opt));
            array_stack_push(stack_opt, c);
            break;
        case NFA_SUBSET_BKT_R:
	    while (!nfa_char_bracket_left_p(array_stack_top(stack_opt))) {
                array_stack_push(stack_data, array_stack_pop(stack_opt));
            }
            array_stack_pop(stack_opt);
            break;
        default:
            assert_exit(false);
            break;
    }
}

static inline void
nfa_engine_re_to_rp_final(char *re, uint32 size,
    s_array_stack_t *stack)
{
    char *tmp;
    uint32 stack_size;

    assert_exit(re);
    assert_exit(stack);

    stack_size = array_stack_size(stack);
    if (size <= stack_size) {
        re[0] = NULL_CHAR;
        assert_exit(false);
    } else {
        re[stack_size] = NULL_CHAR;
        do {
            tmp = array_stack_pop(stack);
            re[--stack_size] = *tmp;
        } while (0 != stack_size);
    }
}

/*
 * re means Regular Expression
 * rp means Reverse Polish Expression
 */
static inline void
nfa_engine_re_to_rp(char *rp, uint32 size, char *re)
{
    char *c, *tmp, top;
    s_array_stack_t *stack_opt, *stack_data;

    assert_exit(re && rp);

    c = re;
    stack_opt = array_stack_create();
    stack_data = array_stack_create();

    while (*c) {
        if (nfa_char_alpha_underline_p(*c)) {
            array_stack_push(stack_data, c);
        } else if (array_stack_empty_p(stack_opt) || NFA_SUBSET_BKT_L == *c) {
            array_stack_push(stack_opt, c);
        } else {
            assert_exit(NFA_SUBSET_BKT_L != *c);
            assert_exit(!array_stack_empty_p(stack_opt));
            tmp = array_stack_top(stack_opt);
            top = *tmp;
            switch (top) {
                case NFA_SUBSET_BKT_L:
                     nfa_engine_re_to_rp_top_priority_0(stack_opt, c);
                     break;
                case NFA_SUBSET_STAR:
                case NFA_SUBSET_PLUS:
                case NFA_SUBSET_QUST:
                    nfa_engine_re_to_rp_top_priority_1(stack_opt, stack_data, c);
                    break;
                case NFA_SUBSET_AND:
                    nfa_engine_re_to_rp_top_priority_2(stack_opt, stack_data, c);
                    break;
                case NFA_SUBSET_OR:
                    nfa_engine_re_to_rp_top_priority_3(stack_opt, stack_data, c);
                    break;
                default:
                    assert_exit(false);
                    break;
            }
        }
        c++;
    }

    while (!array_stack_empty_p(stack_opt)) {
        array_stack_push(stack_data, array_stack_pop(stack_opt));
    }

    nfa_engine_re_to_rp_final(rp, size, stack_data);
    assert_exit(nfa_engine_reverse_polish_legal_p(rp));

    array_stack_destroy(&stack_opt);
    array_stack_destroy(&stack_data);
}

static inline s_nfa_t *
nfa_engine_create_i(char *rp)
{
    char *c;
    s_nfa_t *nfa;
    s_nfa_edge_map_t *map;
    s_array_stack_t *stack;

    assert_exit(rp);

    nfa = NULL;
    c = rp;
    stack = array_stack_create();

    while (*c) {
        if (dp_isalnum(*c) || '_' == *c) {
            map = nfa_edge_map_create(*c, NULL);
            array_stack_push(stack, map);
        } else {
            switch (*c) {
                case NFA_SUBSET_OR:
                case NFA_SUBSET_AND:
                    nfa_subset_rule_induction_binary(stack, *c);
                    break;
                case NFA_SUBSET_STAR:
                case NFA_SUBSET_PLUS:
                case NFA_SUBSET_QUST:
                    nfa_subset_rule_induction_unary(stack, *c);
                    break;
                default:
                    assert_exit(false);
                    break;
            }
        }
        c++;
    }

    map = array_stack_pop(stack);
    assert_exit(array_stack_empty_p(stack));
    array_stack_destroy(&stack);

    nfa = map->nfa;
    // nfa_simplify(nfa);

    map->nfa = NULL;
    nfa_edge_map_destroy(map);

    assert_exit(nfa_engine_graph_legal_p(nfa));
    assert_exit(nfa_engine_structure_legal_p(nfa));
    return nfa;
}

static inline bool
nfa_engine_re_and_needed_p(char last, char c)
{
    if ((nfa_char_unary_opt_p(last) && c == NFA_SUBSET_BKT_L)
        || (nfa_char_alnum_underline_p(last) && NFA_SUBSET_BKT_L == c)
        || (last == NFA_SUBSET_BKT_R && nfa_char_alnum_underline_p(c))
        || (nfa_char_unary_opt_p(last) && nfa_char_alpha_underline_p(c))
        || (nfa_char_alpha_underline_p(last) && nfa_char_alpha_underline_p(c))) {
        return true;
    } else {
        return false;
    }
}

/*
 * re means Regular Expression
 */
static inline void
nfa_engine_re_complete(char *pre, uint32 size, char *re)
{
    char last;
    uint32 index;

    assert_exit(pre && re && size);

    index = 0;
    last = NULL_CHAR;
    while (*re) {
        if (nfa_engine_re_and_needed_p(last, *re)) {
            pre[index++] = NFA_SUBSET_AND;
        }
        pre[index++] = last = *re++;
    }

    if (index >= size) {
        pre[0] = NULL_CHAR;
        assert_exit(false);
    } else {
        pre[index] = NULL_CHAR;
    }
}

/*
 * Regular Expression operator priority:
 *     Level 0: ()      (highest)
 *     Level 1: +, *, ?
 *     Level 2: &
 *     Level 3: |
 */
s_nfa_t *
nfa_engine_create(char *re)
{
    uint32 size;
    char *pre, *rp;
    s_nfa_t *nfa;

    assert_exit(re);

    size = 2 * dp_strlen(re);
    pre = dp_malloc(size);
    rp = dp_malloc(size);

    nfa_engine_re_complete(pre, size, re);
    nfa_engine_re_to_rp(rp, size, pre);
    nfa = nfa_engine_create_i(rp);

    dp_free(pre);
    dp_free(rp);

    return nfa;
}

static inline void
nfa_status_destroy_dfs(s_fa_status_t *status, s_open_addressing_hash_t *hash)
{
    uint32 i;
    void *key;

    assert_exit(status);

    key = (void *)(ptr_t)status->label;

    if (!open_addressing_hash_find(hash, key)) {
        open_addressing_hash_insert(&hash, key);

        i = 0;
        while (i < status->edge_count) {
            assert_exit(status->edge[i]);

            key = (void *)(ptr_t)status->edge[i]->label;
            if (!open_addressing_hash_find(hash, key)) {
                nfa_status_destroy_dfs(status->edge[i]->next, hash);
            }

            dp_free(status->edge[i]);
            i++;
        }
        dp_free(status);
    }
}

void
nfa_engine_destroy(s_nfa_t *nfa)
{
    s_open_addressing_hash_t *hash;

    if (!nfa_engine_structure_legal_p(nfa)) {
        return;
    } else if (!nfa_engine_graph_legal_p(nfa)) {
        return;
    } else {
        hash = open_addressing_hash_create(NFA_LABEL_HASH_SIZE);
        nfa_status_destroy_dfs(nfa->start, hash);
        open_addressing_hash_destroy(&hash);
        dp_free(nfa);
    }
}

static inline void
nfa_engine_array_queue_swap(s_array_queue_t **a, s_array_queue_t **b)
{
    void *tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}

static inline void
nfa_engine_pattern_match_final(s_array_queue_t **master, s_array_queue_t **slave)
{
    uint32 i;
    s_array_queue_t *m;
    s_array_queue_t *s;
    s_fa_status_t *status;

    assert_exit(master && slave && *master && *slave);

    m = *master;
    s = *slave;

    while (!array_queue_empty_p(m)) {
        status = array_queue_leave(m);
        if (0 == status->edge_count) {
            array_queue_enter(s, status);
        } else {
            i = 0;
            while (i < status->edge_count) {
                if (NULL_CHAR == status->edge[i]->c) {
                    array_queue_enter(m, status->edge[i]->next);
                }
                i++;
            }
        }
    }

    *master = s;
    *slave = m;
}

static inline void
nfa_engine_pattern_match_setup(s_array_queue_t *master, s_nfa_t *nfa)
{
    uint32 i;

    assert_exit(master);

    i = 0;
    while (i < nfa->start->edge_count) {
        if (NULL_CHAR == nfa->start->edge[i]->c) {
            array_queue_enter(master, nfa->start->edge[i]->next);
        }
        i++;
    }

    if (array_queue_empty_p(master)) {
        array_queue_enter(master, nfa->start);
    }
}

static inline bool
nfa_engine_pattern_match_ip(s_nfa_t *nfa, char *pn)
{
    uint32 i;
    bool matched;
    char *c, tmp;
    s_fa_status_t *status;
    s_array_queue_t *master, *slave;

    assert_exit(pn);
    assert_exit(nfa_engine_graph_legal_p(nfa));
    assert_exit(nfa_engine_structure_legal_p(nfa));

    c = pn;
    slave = array_queue_create();
    master = array_queue_create();

    nfa_engine_pattern_match_setup(master, nfa);
    while (*c) {
        do {
            i = 0;
            status = array_queue_leave(master);
            while (i < status->edge_count) {
                tmp = status->edge[i]->c;
                if (*c == tmp) {
                    array_queue_enter(slave, status->edge[i]->next);
                } else if (NULL_CHAR == tmp) {
                    array_queue_enter(master, status->edge[i]->next);
                }
                i++;
            }
        } while (!array_queue_empty_p(master));
        nfa_engine_array_queue_swap(&master, &slave);
        c++;
    }

    nfa_engine_pattern_match_final(&master, &slave);
    if (array_queue_empty_p(master)) {
        matched = false;
    } else {
        matched = true;
        assert_exit(array_queue_empty_p(slave));
    }

    array_queue_destroy(&master);
    array_queue_destroy(&slave);
    return matched;
}

bool
nfa_engine_pattern_match_p(s_nfa_t *nfa, char *pn)
{
    if (!pn) {
        return false;
    } else if (!nfa_engine_structure_legal_p(nfa)) {
        return false;
    } else if (!nfa_engine_graph_legal_p(nfa)) {
        return false;
    } else {
        return nfa_engine_pattern_match_ip(nfa, pn);
    }
}

