static inline bool
nfa_closure_structure_legal_p(s_fa_closure_t *closure)
{
    if (!closure) {
        return false;
    } else if (!array_queue_structure_legal_p(closure->path_queue)) {
        return false;
    } else if (!array_queue_structure_legal_p(closure->collection)) {
        return false;
    } else if (!nfa_closure_match_dp_structure_legal_p(closure->match_dp)) {
        return false;
    } else if (bitmap_structure_illegal_p(closure->bitmap)) {
        return false;
    } else {
        return true;
    }
}

static inline bool
nfa_closure_match_dp_structure_legal_p(s_fa_match_dp_t *match_dp)
{
    if (match_dp == NULL) {
        return false;
    } else if (match_dp->dp == NULL) {
        return false;
    } else if (match_dp->index > match_dp->size) {
        return false;
    } else {
        return true;
    }
}

static inline s_fa_match_dp_t *
nfa_closure_match_dp_create(uint32 size)
{
    s_fa_match_dp_t *match_dp;

    assert_exit(size > 0);

    match_dp = dp_malloc(sizeof(*match_dp));
    match_dp->dp = dp_malloc(sizeof(*match_dp->dp) * size);

    match_dp->size = size;
    match_dp->index = 0;

    return match_dp;
}

static inline void
nfa_closure_match_dp_destroy(s_fa_match_dp_t **match_dp)
{
    s_fa_match_dp_t *match_dp_tmp;

    assert_exit(match_dp != NULL);
    assert_exit(nfa_closure_match_dp_structure_legal_p(*match_dp));

    match_dp_tmp = *match_dp;

    dp_free(match_dp_tmp->dp);
    dp_free(match_dp_tmp);

    *match_dp = NULL;
}

static inline void
nfa_closure_match_dp_append(s_nfa_t *nfa, s_fa_closure_t *closure)
{
    uint32 new_sz;
    bool is_matched;
    s_fa_match_dp_t *match_dp;

    assert_exit(nfa_engine_structure_legal_p(nfa));
    assert_exit(nfa_engine_graph_legal_p(nfa));
    assert_exit(nfa_closure_structure_legal_p(closure));

    match_dp = closure->match_dp;
    is_matched = nfa_engine_closure_match_p(nfa, closure);

    if (match_dp->index == match_dp->size) {
        new_sz = match_dp->size * 2 + NFA_MATCH_BUF_MIN;
        match_dp->dp = dp_realloc(match_dp->dp, sizeof(*match_dp->dp) * new_sz);
        match_dp->size = new_sz;
    }

    match_dp->dp[match_dp->index++] = is_matched;
}

static inline s_fa_closure_t *
nfa_closure_create(s_range_uint32_t *range)
{
    s_fa_closure_t *closure;

    assert_exit(range);

    closure = dp_malloc(sizeof(*closure));

    closure->c = NULL_CHAR;
    closure->path_queue = array_queue_create();
    closure->collection = array_queue_create();
    closure->bitmap = bitmap_create(range->min, range->max);
    closure->match_dp = nfa_closure_match_dp_create(NFA_MATCH_BUF_MIN);

    return closure;
}

static inline void
nfa_closure_destroy(s_fa_closure_t **closure)
{
    s_fa_closure_t *closure_tmp;

    assert_exit(closure && *closure);

    closure_tmp = *closure;

    bitmap_destroy(&closure_tmp->bitmap);
    array_queue_destroy(&closure_tmp->path_queue);
    array_queue_destroy(&closure_tmp->collection);

    nfa_closure_match_dp_destroy(&closure_tmp->match_dp);
    dp_free(closure_tmp);

    *closure = NULL;
}

static inline void
nfa_closure_init(s_nfa_t *nfa, s_fa_closure_t *closure)
{
    assert_exit(nfa_engine_structure_legal_p(nfa));
    assert_exit(nfa_engine_graph_legal_p(nfa));
    assert_exit(nfa_closure_structure_legal_p(closure));

    array_queue_enter(closure->collection, nfa->start);
    nfa_closure_null_seek(closure);
}

static inline void
nfa_closure_char_status_seek(s_fa_closure_t *closure, s_fa_status_t *status,
    char c)
{
    s_bitmap_t *bitmap;
    s_array_queue_t *collection;
    s_fa_edge_t *edge, *edge_head;

    if (status->adj_list == NULL) {
        return;
    }

    bitmap = closure->bitmap;
    collection = closure->collection;
    edge = edge_head = status->adj_list;

    do {
        if (c == edge->c && bitmap_bit_clear_p(bitmap, edge->label)) {
            array_queue_enter(collection, edge->succ);
            bitmap_bit_set(bitmap, edge->label);
        }
        edge = nfa_edge_next(edge);
    } while (edge_head != edge);
}

static inline void
nfa_closure_char_seek(s_fa_closure_t *closure, char c)
{
    s_fa_status_t *status;

    assert_exit(c != NULL_CHAR);
    assert_exit(nfa_closure_structure_legal_p(closure));

    closure->c = c;
    swap_pointer((void *)&closure->path_queue, (void *)&closure->collection);

    while (!array_queue_empty_p(closure->path_queue)) {
        status = array_queue_leave(closure->path_queue);
        nfa_closure_char_status_seek(closure, status, c);
    }
}

// Fix-Me, should be array_queue_copy
static inline void
nfa_closure_queue_copy_to_path(s_fa_closure_t *closure)
{
    void *element;
    s_array_queue_t *path_queue;
    s_array_queue_t *collection;
    s_array_iterator_t *iterator;

    assert_exit(nfa_closure_structure_legal_p(closure));

    path_queue = closure->path_queue;
    collection = closure->collection;

    iterator = &path_queue->iterator;
    iterator->fp_index_initial(collection);

    while (iterator->fp_next_exist_p(collection)) {
        element = iterator->fp_next_obtain(collection);
        array_queue_enter(path_queue, element);
    }
}

static inline void
nfa_closure_null_status_seek(s_fa_closure_t *closure, s_fa_status_t *status)
{
    s_bitmap_t *bitmap;
    s_array_queue_t *collection;
    s_array_queue_t *path_queue;
    s_fa_edge_t *edge, *edge_head;

    if (status->adj_list == NULL) {
        return;
    }

    bitmap = closure->bitmap;
    collection = closure->collection;
    path_queue = closure->path_queue;
    edge = edge_head = status->adj_list;

    do {
        if (edge->c == NULL_CHAR && bitmap_bit_clear_p(bitmap, edge->label)) {
            array_queue_enter(path_queue, edge->succ);
            array_queue_enter(collection, edge->succ);
            bitmap_bit_set(bitmap, edge->label);
        }
        edge = nfa_edge_next(edge);
    } while (edge_head != edge);
}

static inline void
nfa_closure_null_seek(s_fa_closure_t *closure)
{
    s_fa_status_t *status;
    s_array_queue_t *path_queue;

    assert_exit(nfa_closure_structure_legal_p(closure));

    nfa_closure_queue_copy_to_path(closure);

    path_queue = closure->path_queue;
    while (!array_queue_empty_p(path_queue)) {
        status = array_queue_leave(path_queue);
        nfa_closure_null_status_seek(closure, status);
    }
}

/*
 * Seek the collection set of nfa engine in 1 step move with char c or NULL_CHAR.
 * 1. Found all status in 1 step move with char c
 * 2. Base on step 1, Merge all status in 1 or more step with NULL_CHAR
 */
static inline void
nfa_closure_seek(s_fa_closure_t *closure, char c)
{
    assert_exit(nfa_closure_structure_legal_p(closure));
    assert_exit(array_queue_empty_p(closure->path_queue));

    bitmap_map_cleanup(closure->bitmap);

    nfa_closure_char_seek(closure, c); // Seek all status of moving 1 step with c
    nfa_closure_null_seek(closure);    // Seek and Merge all status of moving NULL_CHAR
}

static inline uint32
nfa_closure_match_dp_backtrack(s_fa_closure_t *closure)
{
    uint32 i;
    uint32 match_size;
    s_fa_match_dp_t *match_dp;

    assert_exit(nfa_closure_structure_legal_p(closure));

    match_dp = closure->match_dp;
    match_size = closure->match_dp->index;

    while (match_size != 0) {
        i = match_size - 1;

        if (match_dp->dp[i]) {
            return match_size;
        }

        match_size--;
    }

    return NFA_SZ_UNMATCH;
}

static inline bool
nfa_closure_empty_p(s_fa_closure_t *closure)
{
    assert_exit(nfa_closure_structure_legal_p(closure));

    return array_queue_empty_p(closure->collection);
}

