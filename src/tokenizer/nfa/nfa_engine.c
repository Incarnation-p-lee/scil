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
    assert_exit(nfa_status_structure_legal_p(status));
    assert_exit(nfa_engine_structure_legal_p(nfa));

    key = (void *)(ptr_t)status->label;
    assert_exit(PTR_INVALID != open_addressing_hash_find(hash, key));

    if (!open_addressing_hash_find(hash, key)) {
        open_addressing_hash_insert(hash, key);
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
nfa_engine_create_operator(s_array_stack_t *stack, char c)
{
    assert_exit(stack);

    switch (c) {
        case RE_M_OPT_OR:
        case RE_M_OPT_AND:
            nfa_subset_rule_induction_binary(stack, c);
            break;
        case RE_M_OPT_STAR:
        case RE_M_OPT_PLUS:
        case RE_M_OPT_QUST:
            nfa_subset_rule_induction_unary(stack, c);
            break;
        default:
            assert_exit(false);
            break;
    }
}

static inline s_nfa_t *
nfa_engine_create_i(char *polish)
{
    char *c;
    s_nfa_t *nfa;
    s_nfa_edge_map_t *map;
    s_array_stack_t *stack;

    assert_exit(polish);

    nfa = NULL;
    c = polish;
    stack = array_stack_create();
    nfa_label_cleanup();

    while (*c) {
        if (regular_char_data_p(*c)) {
            map = nfa_edge_map_create(*c);
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
nfa_engine_create(char *regular)
{
    s_nfa_t *nfa;
    char *polish;

    assert_exit(regular);

    polish = regular_convert_to_reverse_polish(regular);

    nfa = nfa_engine_create_i(polish);
    nfa_engine_re_copy(nfa, regular);

    dp_free(polish);
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

    assert_exit(nfa_status_structure_legal_p(status));

    key = (void *)(ptr_t)status->label;
    assert_exit(PTR_INVALID != open_addressing_hash_find(hash, key));

    if (!open_addressing_hash_find(hash, key)) {
        open_addressing_hash_insert(hash, key);

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

static inline void
nfa_engine_destroy_final(s_nfa_t *nfa)
{
    assert_exit(nfa);
    assert_exit(NULL == nfa->start && NULL == nfa->terminal);

    if (nfa->re) {
        dp_free(nfa->re);
    }

    dp_free(nfa);
}

static inline void
nfa_engine_destroy_i(s_nfa_t *nfa)
{
    s_open_addressing_hash_t *hash;

    assert_exit(nfa_engine_structure_legal_p(nfa));
    assert_exit(nfa_engine_graph_legal_p(nfa));

    NFA_ENGINE_DESTROY_PRINT(nfa);

    hash = open_addressing_hash_create(NFA_LABEL_HASH_SIZE);
    nfa_status_destroy_dfs(nfa->start, hash);
    open_addressing_hash_destroy(&hash);

    nfa->start = nfa->terminal = NULL;
    nfa_engine_destroy_final(nfa);
}

void
nfa_engine_destroy(s_nfa_t *nfa)
{
    if (!nfa_engine_structure_legal_p(nfa)) {
        return;
    } else if (!nfa_engine_graph_legal_p(nfa)) {
        return;
    } else {
        nfa_engine_destroy_i(nfa);
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
nfa_engine_terminal_reached_p(s_array_queue_t *master)
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

    array_queue_enter(master, nfa->start);
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

    matched = nfa_engine_terminal_reached_p(master);
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

static inline uint32
nfa_engine_token_match_i(s_nfa_t *nfa, char *pn)
{
    char *c;
    uint32 token_size;
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

    while (*c && SENTINEL_CHAR != *c) {
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
            } else {
                token_size = PTR_SIZE_OF(c, pn);
                goto MATCH_DONE;
            }
        }
        nfa_engine_array_queue_swap(&master, &slave);
        c++;
    }

    if (nfa_engine_terminal_reached_p(master)) {
        token_size = PTR_SIZE_OF(c, pn);
    } else {
        token_size = SZ_UNMATCH;
    }

MATCH_DONE:
    array_queue_destroy(&master);
    array_queue_destroy(&slave);
    return token_size;
}

uint32
nfa_engine_token_match(s_nfa_t *nfa, char *pn)
{
    if (!pn) {
        return SZ_UNMATCH;
    } else if (!nfa_engine_structure_legal_p(nfa)) {
        return SZ_UNMATCH;
    } else if (!nfa_engine_graph_legal_p(nfa)) {
        return SZ_UNMATCH;
    } else {
        return nfa_engine_token_match_i(nfa, pn);
    }
}


