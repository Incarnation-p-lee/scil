static inline bool
nfa_engine_structure_legal_p(s_nfa_t *nfa)
{
    if (!nfa) {
        return false;
    } else if (!nfa->start || !nfa->terminal) {
        return false;
    } else if (!nfa_status_legal_p(nfa->start)) {
        return false;
    } else if (!nfa_status_legal_p(nfa->terminal)) {
        return false;
    } else {
        return true;
    }
}

// static inline bool
// nfa_engine_reverse_polish_p(char *rp)
// {
//     if (!rp) {
//         return false;
//     } else {
//         return true;
//     }
// }

static inline void
nfa_engine_regular_to_reverse_polish_top_bkt(s_array_stack_t *stack, char *c)
{
    assert(c);
    assert(stack);
    assert(nfa_engine_stack_opt_top_p(stack, NFA_SUBSET_BKT_L));

    switch (*c) {
        case NFA_SUBSET_BKT_L:
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
            assert(false);
            break;
    }
}

static inline void
nfa_engine_regular_to_reverse_polish_top_opt(s_array_stack_t *stack_opt,
    s_array_stack_t *stack_data, char *c)
{
    char *tmp;

    assert(c && stack_opt && stack_data);
    assert(nfa_engine_stack_opt_top_p(stack_opt, NFA_SUBSET_AND));
    assert(nfa_engine_stack_opt_top_p(stack_opt, NFA_SUBSET_OR));
    assert(nfa_engine_stack_opt_top_p(stack_opt, NFA_SUBSET_STAR));
    assert(nfa_engine_stack_opt_top_p(stack_opt, NFA_SUBSET_PLUS));
    assert(nfa_engine_stack_opt_top_p(stack_opt, NFA_SUBSET_QUST));

    switch (*c) {
        case NFA_SUBSET_BKT_L:
        case NFA_SUBSET_AND:
        case NFA_SUBSET_OR:
        case NFA_SUBSET_STAR:
        case NFA_SUBSET_PLUS:
        case NFA_SUBSET_QUST:
            array_stack_push(stack_opt, c);
            break;
        case NFA_SUBSET_BKT_R:
            tmp = array_stack_pop(stack_opt);
            while (*tmp != NFA_SUBSET_BKT_L) {
                array_stack_push(stack_data, tmp);
                tmp = array_stack_pop(stack_opt);
            }
            break;
        default:
            assert(false);
            break;
    }
}

static inline void
nfa_engine_regular_to_reverse_polish_final(char *re, uint32 size,
    s_array_stack_t *stack)
{
    char *tmp;
    uint32 stack_size;

    assert(re);
    assert(stack);

    stack_size = array_stack_size(stack);
    if (size <= stack_size) {
        re[0] = NULL_CHAR;
        assert(false);
    } else {
        re[stack_size] = NULL_CHAR;
        do {
            tmp = array_stack_pop(stack);
            re[--stack_size] = *tmp;
        } while (0 != stack_size);
    }
}

static inline void
nfa_engine_regular_to_reverse_polish(char *re, uint32 size, char *rp)
{
    char *c, *tmp, top;
    s_array_stack_t *stack_opt, *stack_data;

    assert(re && rp);

    c = rp;
    stack_opt = array_stack_create();
    stack_data = array_stack_create();

    while (*c) {
        if (dp_isalnum(*c)) {
            array_stack_push(stack_data, c);
        } else {
            tmp = array_stack_top(stack_opt);
            top = tmp ? *tmp : NULL_CHAR;
            switch (top) {
                case NFA_SUBSET_BKT_L:
                     nfa_engine_regular_to_reverse_polish_top_bkt(stack_opt, c);
                     break;
                case NFA_SUBSET_AND:
                case NFA_SUBSET_OR:
                case NFA_SUBSET_STAR:
                case NFA_SUBSET_PLUS:
                case NFA_SUBSET_QUST:
                    nfa_engine_regular_to_reverse_polish_top_opt(stack_opt, stack_data, c);
                    break;
                default:
                    break;
            }
        }
        c++;
    }

    while (array_stack_empty_p(stack_opt)) {
        array_stack_push(stack_data, array_stack_pop(stack_opt));
    }

    nfa_engine_regular_to_reverse_polish_final(re, size, stack_data);

    array_stack_destroy(&stack_opt);
    array_stack_destroy(&stack_data);
}

s_nfa_t *
nfa_engine_create(char *re)
{
    char *c;
    s_array_stack_t *stack;
    s_nfa_t *nfa, *nfa_tmp;
    s_nfa_edge_map_t *map, *map_tmp;

    nfa = NULL;
    // re should be Reverse Polish Expression
    // Add checking here.
    if (re) {
        c = re;
        stack = array_stack_create();

        while (*c) {
            if (dp_isalnum(*c) || '_' == *c) {
                map = nfa_edge_map_create(*c, NULL);
                array_stack_push(stack, map);
            } else {
                switch (*c) {
                    case NFA_SUBSET_OR:
                    case NFA_SUBSET_AND:
                        map = array_stack_pop(stack);
                        map_tmp = array_stack_pop(stack);
                        nfa = nfa_edge_map_nfa_obtain(map);
                        nfa_tmp = nfa_edge_map_nfa_obtain(map_tmp);
                        nfa_subset_rule_induction_binary(nfa, nfa_tmp, *c);
                        nfa_edge_map_destroy(map_tmp);
                        array_stack_push(stack, map);
                        break;
                    case NFA_SUBSET_STAR:
                    case NFA_SUBSET_PLUS:
                    case NFA_SUBSET_QUST:
                        map = array_stack_pop(stack);
                        nfa = nfa_edge_map_nfa_obtain(map);
                        nfa_subset_rule_induction_unary(nfa, *c);
                        array_stack_push(stack, map);
                        break;
                    default:
                        assert(false);
                        break;
                }
            }
            c++;
        }

        map = array_stack_pop(stack);
        assert(array_stack_empty_p(stack));

        nfa = map->nfa;
        map->nfa = NULL;
        nfa_edge_map_destroy(map);
        array_stack_destroy(&stack);
    }

    assert(nfa_engine_graph_legal_p(nfa));
    assert(nfa_engine_structure_legal_p(nfa));
    return nfa;
}

static inline void
nfa_status_destroy_dfs(s_fa_status_t *status, s_open_addressing_hash_t *hash)
{
    uint32 i;
    void *key;

    assert(status);

    key = (void *)(ptr_t)status->label;

    if (!open_addressing_hash_find(hash, key)) {
        open_addressing_hash_insert(&hash, key);

        i = 0;
        while (i < status->edge_count) {
            assert(status->edge[i]);
            nfa_status_destroy_dfs(status->edge[i]->next, hash);
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

