bool
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
        case RE_WILD_OR:
        case RE_WILD_AND:
            nfa_subset_rule_induction_binary(stack, c);
            break;
        case RE_WILD_STAR:
        case RE_WILD_PLUS:
        case RE_WILD_QUST:
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

    nfa_label_cleanup();

    c = polish;
    stack = array_stack_create();

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

    nfa = map->nfa;
    nfa_simplify(nfa);
    nfa_label_range_set(nfa);

    nfa_edge_map_destroy(map);
    array_stack_destroy(&stack);

    assert_exit(nfa_engine_structure_legal_p(nfa));
    assert_exit(nfa_engine_graph_legal_p(nfa));
    return nfa;
}

static inline void
nfa_engine_re_copy(s_nfa_t *nfa, char *re)
{
    uint32 len;

    assert_exit(re);
    assert_exit(nfa_engine_structure_legal_p(nfa));

    len = dp_strlen(re);
    nfa->re = dp_malloc(sizeof(*re) * (len + 1));
    dp_memcpy(nfa->re, re, len);
    nfa->re[len] = NULL_CHAR;
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
nfa_engine_status_destroy_dfs(s_fa_status_t *status, s_open_addressing_hash_t *hash)
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
                    nfa_engine_status_destroy_dfs(edge->succ, hash);
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
    nfa_engine_status_destroy_dfs(nfa->start, hash);
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

static inline bool
nfa_engine_closure_match_p(s_nfa_t *nfa, s_fa_closure_t *closure)
{
    s_bitmap_t *bitmap;
    s_fa_status_t *status;

    assert_exit(nfa_engine_structure_legal_p(nfa));
    assert_exit(nfa_closure_structure_legal_p(closure));

    bitmap = closure->bitmap;
    status = nfa->terminal;

    NFA_ENGINE_CLOSURE_MATCH_PRINT(nfa, closure);
    return bitmap_bit_set_p(bitmap, status->label);
}

static inline void
nfa_engine_patern_match_char_mov(s_fa_closure_t *closure, char c)
{
    assert_exit(nfa_closure_structure_legal_p(closure));
    assert_exit(array_queue_empty_p(closure->path_queue));

    nfa_closure_seek(closure, c);
}

static inline uint32
nfa_engine_pattern_match_ip(s_nfa_t *nfa, char *pn)
{
    char *c;
    bool is_matched;
    s_fa_closure_t *closure;

    assert_exit(pn);
    assert_exit(nfa_engine_structure_legal_p(nfa));
    assert_exit(nfa_engine_graph_legal_p(nfa));

    closure = nfa_closure_create(&nfa->label_range);
    nfa_closure_init(nfa, closure);

    c = pn;
    NFA_CLOSURE_PRINT(closure, pn);

    while (*c) {
        nfa_engine_patern_match_char_mov(closure, *c);
        NFA_CLOSURE_PRINT(closure, pn);
        c++;
    }

    is_matched = nfa_engine_closure_match_p(nfa, closure);
    nfa_closure_destroy(&closure);

    return is_matched;
}

static inline uint32
nfa_engine_pattern_match_i(s_nfa_t *nfa, char *pn)
{
    char *c;
    uint32 match_size;
    s_fa_closure_t *closure;

    assert_exit(pn);
    assert_exit(nfa_engine_structure_legal_p(nfa));
    assert_exit(nfa_engine_graph_legal_p(nfa));

    closure = nfa_closure_create(&nfa->label_range);
    nfa_closure_init(nfa, closure);
    NFA_CLOSURE_PRINT(closure, pn);

    c = pn;
    while (*c) {
        nfa_engine_patern_match_char_mov(closure, *c);
        NFA_CLOSURE_PRINT(closure, pn);

        nfa_closure_match_dp_append(nfa, closure);
        c++;
    }

    match_size = nfa_closure_match_dp_backtrack(closure);
    nfa_closure_destroy(&closure);

    return match_size;
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

uint32
nfa_engine_pattern_match(s_nfa_t *nfa, char *pn)
{
    if (!pn) {
        return NFA_SZ_INVALID;
    } else if (!nfa_engine_structure_legal_p(nfa)) {
        return NFA_SZ_INVALID;
    } else if (!nfa_engine_graph_legal_p(nfa)) {
        return NFA_SZ_INVALID;
    } else {
        return nfa_engine_pattern_match_i(nfa, pn);
    }
}

