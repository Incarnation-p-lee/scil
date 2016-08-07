static inline s_nfa_edge_map_t *
nfa_edge_map_create(char c, s_nfa_t *nfa)
{
    s_nfa_edge_map_t *map;

    map = dp_malloc(sizeof(*map));
    map->c = c;
    map->nfa = nfa;

    return map;
}

static inline s_fa_edge_t *
nfa_edge_next(s_fa_edge_t *edge)
{
    assert_exit(edge);

    return CONTAINS_OF(edge->list.next, s_fa_edge_t, list);
}

static inline void
nfa_edge_map_destroy(s_nfa_edge_map_t *map)
{
    assert_exit(NULL != map);

    map->nfa = NULL;
    dp_free(map);
}

static inline s_nfa_t *
nfa_edge_map_nfa_obtain(s_nfa_edge_map_t *map)
{
    assert_exit(NULL != map);

    if (!map->nfa) {
        map->nfa = nfa_subset_rule_basic(map->c);
    }

    return map->nfa;
}

static inline uint32
nfa_label_obtain(void)
{
    return nfa_status_lalel++;
}

static inline void
nfa_label_cleanup(void)
{
    nfa_status_lalel = 1;
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

static inline bool
nfa_char_bracket_left_p(char *c)
{
    assert_exit(c);

    if (NFA_SUBSET_BKT_L == *c) {
        return true;
    } else {
        return false;
    }
}

static inline bool
nfa_char_alpha_underline_p(char c)
{
    if (dp_isalpha(c) || '_' == c) {
        return true;
    } else {
        return false;
    }
}

static inline bool
nfa_char_bracket_p(char c)
{
    if (c == NFA_SUBSET_BKT_R || c == NFA_SUBSET_BKT_L) {
        return true;
    } else {
        return false;
    }
}

static inline bool
nfa_char_unary_opt_p(char c)
{
    if (NFA_SUBSET_STAR == c || NFA_SUBSET_PLUS == c || NFA_SUBSET_QUST == c) {
        return true;
    } else {
        return false;
    }
}

static inline bool
nfa_char_binary_opt_p(char c)
{
    if (NFA_SUBSET_OR == c || NFA_SUBSET_AND == c) {
        return true;
    } else {
        return false;
    }
}

static inline bool
nfa_char_operator_p(char c)
{
    if (nfa_char_binary_opt_p(c)) {
        return true;
    } else if (nfa_char_unary_opt_p(c)) {
        return true;
    } else if (nfa_char_bracket_p(c)) {
        return true;
    } else {
        return false;
    }
}

static inline bool
nfa_char_alnum_underline_p(char c)
{
    if (isalnum(c) || '_' == c) {
        return true;
    } else {
        return false;
    }
}

