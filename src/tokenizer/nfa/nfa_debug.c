static inline bool
nfa_engine_graph_legal_p(s_nfa_t *nfa)
{
    bool legal;
    s_open_addressing_hash_t *hash;

    assert(nfa_engine_structure_legal_p(nfa));

    hash = open_addressing_hash_create(NFA_LABEL_HASH_SIZE);
    legal = nfa_status_dfs_reached_p(nfa->start, nfa->terminal, hash);
    open_addressing_hash_destroy(&hash);

    return legal;
}

static inline bool
nfa_engine_stack_opt_top_p(s_array_stack_t *stack, char expected)
{
    char *tmp;

    assert(stack);

    tmp = array_stack_top(stack);

    return *tmp == expected;
}

static inline bool
nfa_status_dfs_reached_p(s_fa_status_t *start, s_fa_status_t *terminal,
    s_open_addressing_hash_t *hash)
{
    uint32 i;
    void *key;

    assert(hash);
    assert(start);
    assert(terminal);
    assert(!terminal->edge_count);

    key = (void *)(ptr_t)start->label;

    if (!open_addressing_hash_find(hash, key)) {
        open_addressing_hash_insert(&hash, key);
        if (start == terminal) {
            return true;
        }

        i = 0;
        while (i < start->edge_count) {
            assert(start->edge[i]);
            if (nfa_status_dfs_reached_p(start->edge[i]->next, terminal, hash)) {
                return true;
            }
            i++;
        }
    }

    return false;
}

