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

    scil_log_print(">> DESTROY NFA engine '%s'\n\n", nfa->re);
}

static inline void
nfa_engine_graph_print(s_nfa_t *nfa)
{
    s_open_addressing_hash_t *hash;

    assert_exit(nfa_engine_structure_legal_p(nfa));

    scil_log_print(">> NFA engine graph print for regular expression '%s'\n", nfa->re);

    hash = open_addressing_hash_create(NFA_LABEL_HASH_SIZE);
    nfa_engine_graph_dfs_print(nfa->start, hash);
    open_addressing_hash_destroy(&hash);
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
nfa_closure_title_print(char c)
{
    if (c == NULL_CHAR) {
        scil_log_print(">> NFA closure collection *START*\n");
    } else {
        scil_log_print(">> NFA closure collection '%c'\n", c);
    }
}

static inline void
nfa_closure_print(s_fa_closure_t *closure)
{
    uint32 i;
    s_fa_status_t *status;
    s_array_queue_t *collection;
    s_array_iterator_t *iterator;

    assert_exit(nfa_closure_structure_legal_p(closure));

    nfa_closure_title_print(closure->c);

    collection = closure->collection;
    if (array_queue_empty_p(collection)) {
        return;
    }

    iterator = array_queue_iterator_obtain(collection);
    iterator->fp_index_initial(collection);

    i = 0;
    scil_log_print("    ");

    while (iterator->fp_next_exist_p(collection)) {
        status = iterator->fp_next_obtain(collection);
        scil_log_print("%04d, ", status->label);

        i++;
        if (i == NFA_CLOSURE_PRINT_LEN) {
            i = 0;
            scil_log_print("\n    ");
        }
    }

    if (i) {
        scil_log_print("\n");
    }
}

static inline void
nfa_engine_closure_match_print(s_nfa_t *nfa, s_fa_closure_t *closure)
{
    s_bitmap_t *bitmap;
    s_fa_status_t *status;

    assert_exit(nfa_engine_structure_legal_p(nfa));
    assert_exit(nfa_closure_structure_legal_p(closure));

    bitmap = closure->bitmap;
    status = nfa->terminal;

    if (bitmap_bit_set_p(bitmap, status->label)) {
        scil_log_print("## Matched *TERMINAL* %04d in closure at '%c'\n",
            status->label, closure->c);
    }
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

static inline bool
nfa_engine_graph_illegal_p(s_nfa_t *nfa)
{
    return !nfa_engine_graph_legal_p(nfa);
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

