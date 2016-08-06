static inline bool
nfa_engine_stack_top_p(s_array_stack_t *stack, char expected)
{
    char *tmp;

    assert_exit(stack);

    tmp = array_stack_top(stack);

    return *tmp == expected;
}

static inline void
nfa_engine_graph_print_status(s_fa_status_t *status)
{
    char tmp;
    uint32 edge_idx;
    s_fa_edge_t *edge, *edge_head;

    assert_exit(nfa_status_structure_legal_p(status));

    if (!status->adj_list) {
        log_print("    Status %#x [%d] *TERMINAL* \n", status, status->label);
    } else {
        edge_idx = 0;
        edge = edge_head = status->adj_list;
        log_print("    Status %p [%d]\n", status, status->label);

        do {
            tmp = NULL_CHAR == edge->c ? STAR_CHAR : edge->c;
            log_print("        |-edges %02u: c = %c label = %03d succ = %03d\n",
                edge_idx++, tmp, edge->label, edge->succ->label);
            edge = nfa_edge_next(edge);
        } while (edge_head != edge);
    }
}


static inline void
nfa_engine_graph_dfs_print(s_fa_status_t *status, s_open_addressing_hash_t *hash)
{
    void *key;
    s_fa_edge_t *edge, *edge_head;

    assert_exit(hash);
    assert_exit(nfa_status_structure_legal_p(status));

    key = (void *)(ptr_t)status->label;

    if (!open_addressing_hash_find(hash, key)) {
        open_addressing_hash_insert(&hash, key);
        nfa_engine_graph_print_status(status);

        if (status->adj_list) {
            edge = edge_head = status->adj_list;
            do {
                nfa_engine_graph_dfs_print(edge->succ, hash);
                edge = nfa_edge_next(edge);
            } while (edge_head != edge);
        }
    }
}

static inline void
nfa_engine_graph_print(s_nfa_t *nfa)
{
    s_open_addressing_hash_t *hash;

    assert_exit(nfa_engine_structure_legal_p(nfa));

    log_print("\n<NFA engine graph print for regular expression '%s'>\n", nfa->re);

    hash = open_addressing_hash_create(NFA_LABEL_HASH_SIZE);
    nfa_engine_graph_dfs_print(nfa->start, hash);
    open_addressing_hash_destroy(&hash);

    log_print(">> END of NFA engine graph print '%s' <<\n\n", nfa->re);
}

static inline bool
nfa_engine_reverse_polish_legal_p(char *rp)
{
    char *c;
    char tmp;
    bool is_legal;
    s_array_stack_t *stack;

    assert_exit(rp);

    c = rp;
    tmp = NULL_CHAR;
    stack = array_stack_create();

    while (*c) {
        if (nfa_char_alnum_underline_p(*c)) {
            array_stack_push(stack, &tmp);
        } else if (nfa_char_binary_opt_p(*c)) {
            array_stack_pop(stack);
            array_stack_pop(stack);
            array_stack_push(stack, &tmp);
        } else if (nfa_char_unary_opt_p(*c)) {
            array_stack_pop(stack);
            array_stack_push(stack, &tmp);
        } else {
            assert_exit(false);
        }
        c++;
    }

    array_stack_pop(stack);
    if (array_stack_empty_p(stack)) {
        is_legal = true;
    } else {
        is_legal = false;
    }

    array_stack_destroy(&stack);
    return is_legal;
}

static inline bool
nfa_status_terminal_p(s_fa_status_t *status)
{
    assert_exit(nfa_status_structure_legal_p(status));

    if (!status->adj_list) {
        return true;
    } else {
        return false;
    }
}

