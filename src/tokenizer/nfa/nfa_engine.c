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
nfa_engine_graph_dfs_reached_p(s_nfa_t *nfa,
    s_open_addressing_hash_t *hash, s_fa_status_t *status)
{
    void *key;
    s_fa_edge_t *edge;
    s_fa_edge_t *edge_head;
    s_fa_status_t *succ;

    assert_exit(hash);
    assert_exit(status);
    assert_exit(nfa_engine_structure_legal_p(nfa));

    key = (void *)(ptr_t)status->label;

    if (!open_addressing_hash_find(hash, key)) {
        open_addressing_hash_insert(&hash, key);
        if (status == nfa->terminal) {
            return true;
        }

        edge = edge_head = status->adj_list;
        do {
            succ = edge->succ;
            if (succ == nfa->start) {
                return false; /* Start status should not have enter edge */
            } else if (nfa_engine_graph_dfs_reached_p(nfa, hash, succ)) {
                return true;
            }
            edge = nfa_edge_next(edge);
        } while (edge_head != edge);
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
    legal = nfa_engine_graph_dfs_reached_p(nfa, hash, nfa->start);
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
            array_stack_pop(stack_opt);
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
        assert_exit(size > stack_size);
    } else {
        re[stack_size] = NULL_CHAR;
        do {
            tmp = array_stack_pop(stack);
            re[--stack_size] = *tmp;
        } while (0 != stack_size);
    }
}

static inline void
nfa_engine_re_to_rp_operator(s_array_stack_t *stack_data,
    s_array_stack_t *stack_opt, char *c)
{
    char *top;

    assert_exit(c);
    assert_exit(stack_opt);
    assert_exit(stack_data);
    assert_exit(nfa_char_operator_p(*c));
    assert_exit(!array_stack_empty_p(stack_opt));

    top = array_stack_top(stack_opt);

    switch (*top) {
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

/*
 * re means Regular Expression
 * rp means Reverse Polish Expression
 */
static inline char *
nfa_engine_re_to_rp(char *pre_re)
{
    uint32 size;
    char *c, *rp;
    s_array_stack_t *stack_opt, *stack_data;

    assert_exit(pre_re);

    c = pre_re;
    stack_opt = array_stack_create();
    stack_data = array_stack_create();

    while (*c) {
        if (nfa_char_alpha_underline_p(*c)) {
            array_stack_push(stack_data, c);
        } else if (array_stack_empty_p(stack_opt) || NFA_SUBSET_BKT_L == *c) {
            array_stack_push(stack_opt, c);
        } else {
            nfa_engine_re_to_rp_operator(stack_data, stack_opt, c);
        }
        c++;
    }

    while (!array_stack_empty_p(stack_opt)) {
        array_stack_push(stack_data, array_stack_pop(stack_opt));
    }

    size = dp_strlen(pre_re) + 1;
    rp = dp_malloc(sizeof(char) * size);
    assert_exit(size > array_stack_size(stack_data));

    nfa_engine_re_to_rp_final(rp, size, stack_data);
    assert_exit(nfa_engine_reverse_polish_legal_p(rp));

    array_stack_destroy(&stack_opt);
    array_stack_destroy(&stack_data);

    return rp;
}

static inline void
nfa_engine_create_operator(s_array_stack_t *stack, char c)
{
    assert_exit(stack);

    switch (c) {
        case NFA_SUBSET_OR:
        case NFA_SUBSET_AND:
            nfa_subset_rule_induction_binary(stack, c);
            break;
        case NFA_SUBSET_STAR:
        case NFA_SUBSET_PLUS:
        case NFA_SUBSET_QUST:
            nfa_subset_rule_induction_unary(stack, c);
            break;
        default:
            assert_exit(false);
            break;
    }
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
    nfa_label_cleanup();

    while (*c) {
        if (dp_isalnum(*c) || '_' == *c) {
            map = nfa_edge_map_create(*c, NULL);
            array_stack_push(stack, map);
        } else {
            nfa_engine_create_operator(stack, *c);
        }
        c++;
    }

    map = array_stack_pop(stack);
    assert_exit(array_stack_empty_p(stack));
    array_stack_destroy(&stack);

    nfa = map->nfa;
    nfa_edge_map_destroy(map);

    assert_exit(nfa_engine_structure_legal_p(nfa));
    assert_exit(nfa_engine_graph_legal_p(nfa));
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
static inline char *
nfa_engine_re_preprocessing(char *re)
{
    char last, *pre;
    uint32 index, pre_size;

    assert_exit(re);

    index = 0;
    last = NULL_CHAR;
    pre_size = dp_strlen(re) * 2 + NFA_RE_SIZE_MIN;
    pre = dp_malloc(pre_size);

    while (*re) {
        if (nfa_engine_re_and_needed_p(last, *re)) {
            pre[index++] = NFA_SUBSET_AND;
        }
        pre[index++] = last = *re++;

        if (index + 3 >= pre_size) {
            pre_size = pre_size * 2;
            pre = dp_realloc(pre, sizeof(char) * pre_size);
        }
    }

    pre[index] = NULL_CHAR;
    assert_exit(pre);
    return pre;
}

static inline void
nfa_engine_re_copy(s_nfa_t *nfa, char *re)
{
    uint32 l;

    assert_exit(re);
    assert_exit(nfa_engine_structure_legal_p(nfa));

    l = dp_strlen(re);
    nfa->re = dp_malloc(sizeof(*re) * (l + 1));
    dp_memcpy(nfa->re, re, l);
    nfa->re[l] = NULL_CHAR;
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
    s_nfa_t *nfa;
    char *pre, *rp;

    assert_exit(re);

    pre = nfa_engine_re_preprocessing(re);
    rp = nfa_engine_re_to_rp(pre);
    dp_free(pre);

    nfa = nfa_engine_create_i(rp);
    nfa_engine_re_copy(nfa, re);
    dp_free(rp);

    NFA_ENGINE_GRAPH_PRINT(nfa);
    return nfa;
}

static inline void
nfa_status_destroy_dfs(s_fa_status_t *status, s_open_addressing_hash_t *hash)
{
    void *key;
    s_fa_edge_t *edge;
    s_fa_edge_t *edge_head;
    s_fa_edge_t *edge_next;

    assert_exit(status);

    key = (void *)(ptr_t)status->label;

    if (!open_addressing_hash_find(hash, key)) {
        open_addressing_hash_insert(&hash, key);

        if (status->adj_list) {
            edge = edge_head = status->adj_list;
            do {
                key = (void *)(ptr_t)edge->label;
                if (!open_addressing_hash_find(hash, key)) {
                    nfa_status_destroy_dfs(edge->succ, hash);
                }

                edge_next = nfa_edge_next(edge);
                dp_free(edge);
                edge = edge_next;
            } while (edge_head != edge);
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

        dp_free(nfa->re);
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

static inline bool
nfa_engine_pattern_match_final_p(s_array_queue_t *master)
{
    s_fa_status_t *status;
    s_fa_edge_t *edge, *edge_head;

    assert_exit(master);

    while (!array_queue_empty_p(master)) {
        status = array_queue_leave(master);
        if (!status->adj_list) {
            return true;
        } else {
            edge = edge_head = status->adj_list;
            do {
                if (NULL_CHAR == edge->c) {
                    array_queue_enter(master, edge->succ);
                }
                edge = nfa_edge_next(edge);
            } while (edge_head != edge);
        }
    }

    return false;
}

static inline void
nfa_engine_pattern_match_setup(s_array_queue_t *master, s_nfa_t *nfa)
{
    s_fa_edge_t *edge, *edge_head;

    assert_exit(master);
    assert_exit(nfa_engine_structure_legal_p(nfa));

    edge = edge_head = nfa->start->adj_list;

    do {
        if (NULL_CHAR == edge->c) {
            array_queue_enter(master, edge->succ);
        }
        edge = nfa_edge_next(edge);
    } while (edge_head != edge);

    if (array_queue_empty_p(master)) {
        array_queue_enter(master, nfa->start);
    }
}

static inline bool
nfa_engine_pattern_match_ip(s_nfa_t *nfa, char *pn)
{
    char *c;
    bool matched;
    s_fa_status_t *status;
    s_array_queue_t *master, *slave;
    s_fa_edge_t *edge, *edge_head;

    assert_exit(pn);
    assert_exit(nfa_engine_graph_legal_p(nfa));
    assert_exit(nfa_engine_structure_legal_p(nfa));

    c = pn;
    slave = array_queue_create();
    master = array_queue_create();
    nfa_engine_pattern_match_setup(master, nfa);

    while (*c) {
        while (!array_queue_empty_p(master)) {
            status = array_queue_leave(master);
            if (status->adj_list) {
                edge = edge_head = status->adj_list;
                do {
                    if (*c == edge->c) {
                        array_queue_enter(slave, edge->succ);
                    } else if (NULL_CHAR == edge->c) {
                        array_queue_enter(master, edge->succ);
                    }
                    edge = nfa_edge_next(edge);
                } while (edge_head != edge);
            }
        }
        nfa_engine_array_queue_swap(&master, &slave);
        c++;
    }

    matched = nfa_engine_pattern_match_final_p(master);
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

