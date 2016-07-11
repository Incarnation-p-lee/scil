static inline bool
nfa_engine_legal_p(s_nfa_t *nfa)
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
                        dp_assert(false);
                        break;
                }
            }
            c++;
        }

        map = array_stack_pop(stack);
        dp_assert(array_stack_empty_p(stack));

        nfa = map->nfa;
        map->nfa = NULL;
        nfa_edge_map_destroy(map);
        array_stack_destroy(&stack);
    }

    dp_assert(nfa_engine_legal_p(nfa));
    return nfa;
}

static inline void
nfa_status_destroy_dfs(s_fa_status_t *status, s_open_addressing_hash_t *hash)
{
    uint32 i;
    void *key;

    dp_assert(status);

    key = (void *)(ptr_t)status->label;

    if (!open_addressing_hash_find(hash, key)) {
        open_addressing_hash_insert(&hash, key);

        i = 0;
        while (i < status->edge_count) {
            dp_assert(status->edge[i]);
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

    if (!nfa_engine_legal_p(nfa)) {
        return;
    } else {
        hash = open_addressing_hash_create(NFA_LABEL_HASH_SIZE);
        nfa_status_destroy_dfs(nfa->start, hash);
        open_addressing_hash_destroy(&hash);
        dp_free(nfa);
    }
}

