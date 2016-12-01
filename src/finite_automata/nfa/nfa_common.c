static inline s_nfa_t *
nfa_char_data_create(char c)
{
    assert_exit(regular_char_data_p(c));

    if (regular_char_translated_p(c)) {
        c = regular_char_translate_resume(c);
    }

    return nfa_subset_rule_basic(c);
}

static inline s_fa_edge_t *
nfa_edge_next(s_fa_edge_t *edge)
{
    assert_exit(edge);

    return CONTAINS_OF(edge->list.next, s_fa_edge_t, list);
}

static inline uint32
nfa_label_obtain(void)
{
    return nfa_status_lalel++;
}

static inline void
nfa_label_cleanup(void)
{
    nfa_status_lalel = NFA_LABEL_MIN;
}

static inline void
nfa_label_range_set(s_nfa_t *nfa)
{
    assert_exit(nfa_engine_structure_legal_p(nfa));

    // For convention, the min of nfa label range is always NFA_LABEL_MIN.
    nfa->label_range.min = NFA_LABEL_MIN;
    nfa->label_range.max = nfa_status_lalel;
}

static inline s_fa_status_t *
nfa_status_create(void)
{
    s_fa_status_t *retval;

    retval = dp_malloc(sizeof(*retval));
    assert_exit(NULL != retval);

    retval->label = nfa_label_obtain();
    retval->adj_list = NULL;

    return retval;
}

/*
 * Merge the adj_queue from m => t.
 */
static inline void
nfa_status_terminal_merge(s_fa_status_t *t, s_fa_status_t *m)
{
    assert_exit(nfa_status_structure_legal_p(t));
    assert_exit(nfa_status_structure_legal_p(m));
    assert_exit(nfa_status_terminal_p(t));

    t->adj_list = m->adj_list;
    m->adj_list = NULL;

    dp_free(m);
}

static inline void
nfa_status_edge_chain(s_fa_status_t *status, char c, s_fa_status_t *succ)
{
    s_fa_edge_t *edge;

    assert_exit(nfa_status_structure_legal_p(succ));
    assert_exit(nfa_status_structure_legal_p(status));

    edge = dp_malloc(sizeof(s_fa_edge_t));
    edge->c = c;
    edge->succ = succ;
    edge->label = succ->label;
    doubly_linked_list_initial(&edge->list);

    if (status->adj_list) {
        doubly_linked_list_insert_before(&status->adj_list->list, &edge->list);
    } else {
        status->adj_list = edge;
    }
}

