static inline void
nfa_engine_graph_print_status(s_fa_status_t *status)
{
    char tmp;
    uint32 edge_idx;
    s_fa_edge_t *edge, *edge_head;

    assert_exit(nfa_status_structure_legal_p(status));

    if (!status->adj_list) {
        scil_log_print("    Status %p [%d] *TERMINAL* \n", status, status->label);
    } else {
        edge_idx = 0;
        edge = edge_head = status->adj_list;
        scil_log_print("    Status %p [%d]\n", status, status->label);

        do {
            tmp = NULL_CHAR == edge->c ? NFA_MARK_CHAR : edge->c;
            scil_log_print("        |-edges %02u [label %03d]: c = '%c' => succ = %p\n",
                edge_idx++, edge->label, tmp, edge->succ);
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
    assert_exit(PTR_INVALID != open_addressing_hash_find(hash, key));

    if (!open_addressing_hash_find(hash, key)) {
        open_addressing_hash_insert(hash, key);
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
nfa_engine_destroy_print(s_nfa_t *nfa)
{
    assert_exit(nfa_engine_structure_legal_p(nfa));

    scil_log_print("\n=====> DESTROY NFA engine '%s'\n\n", nfa->re);
}

static inline void
nfa_engine_graph_print(s_nfa_t *nfa)
{
    s_open_addressing_hash_t *hash;

    assert_exit(nfa_engine_structure_legal_p(nfa));

    scil_log_print("\n<NFA engine graph print for regular expression '%s'>\n", nfa->re);

    hash = open_addressing_hash_create(NFA_LABEL_HASH_SIZE);
    nfa_engine_graph_dfs_print(nfa->start, hash);
    open_addressing_hash_destroy(&hash);

    scil_log_print(">> END of NFA engine graph print '%s' <<\n\n", nfa->re);
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

static inline void
nfa_closure_print(s_fa_closure_t *closure)
{
    char c;
    uint32 i;
    s_fa_status_t *status;
    s_array_queue_t *collection;
    s_array_iterator_t *iterator;

    assert_exit(nfa_closure_structure_legal_p(closure));

    c = closure->c;

    if (c == NULL_CHAR) {
        scil_log_print("\n<NFA closure collection of start status>\n[\n ");
    } else {
        scil_log_print("\n<NFA closure collection of '%c'>\n[\n ", c);
    }

    i = 0;
    collection = closure->collection;
    iterator = array_queue_iterator_obtain(collection);
    iterator->fp_index_initial(collection);

    while (iterator->fp_next_exist_p(collection)) {
        status = iterator->fp_next_obtain(collection);
        scil_log_print("%04d, ", status->label);

        i++;
        if (i == NFA_CLOSURE_PRINT_LEN) {
            i = 0;
            scil_log_print("\n ");
        }
    }

    scil_log_print("\n]\n<End of NFA closure collection>\n\n");
}

