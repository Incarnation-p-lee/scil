static inline s_nfa_edge_map_t *
nfa_edge_map_create(char c, s_nfa_t *nfa)
{
    s_nfa_edge_map_t *map;

    map = dp_malloc(sizeof(*map));
    map->c = c;
    map->nfa = nfa;

    return map;
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
    nfa_status_lalel = 0;
}

static inline s_fa_status_t *
nfa_status_create(void)
{
    s_fa_status_t *retval;

    retval = dp_malloc(sizeof(*retval));
    assert_exit(NULL != retval);

    retval->edge_count = 0;
    retval->label = nfa_label_obtain();

    dp_memset(retval->edge, 0, sizeof(retval->edge));
    return retval;
}

static inline void
nfa_status_terminal_merge(s_fa_status_t *t, s_fa_status_t *m)
{
    uint32 index;

    assert_exit(nfa_status_structure_legal_p(t));
    assert_exit(t->edge_count == 0);
    assert_exit(nfa_status_structure_legal_p(m));

    index = 0;
    while (index < m->edge_count) {
        t->edge[t->edge_count++] = m->edge[index];
        m->edge[index++] = NULL;
    }

    dp_free(m);
}

static inline void
nfa_status_edge_chain(s_fa_status_t *status, char c, s_fa_status_t *next)
{
    uint32 index;

    assert_exit(NULL != next);
    assert_exit(NULL != status);
    assert_exit(NULL == status->edge[status->edge_count]);
    assert_exit(status->edge_count < NFA_EDGE_MAX - 1);

    index = status->edge_count++;
    status->edge[index] = dp_malloc(sizeof(s_fa_edge_t));
    status->edge[index]->c = c;
    status->edge[index]->next = next;
    status->edge[index]->label = next->label;
}

static inline s_nfa_t *
nfa_create(void)
{
    s_nfa_t *nfa;

    nfa = dp_malloc(sizeof(s_nfa_t));
    nfa->start = nfa->terminal = NULL;

    return nfa;
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

