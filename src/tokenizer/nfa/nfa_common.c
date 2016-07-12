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
    assert(NULL != map);

    map->nfa = NULL;
    dp_free(map);
}

static inline s_nfa_t *
nfa_edge_map_nfa_obtain(s_nfa_edge_map_t *map)
{
    assert(NULL != map);

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
    assert(NULL != retval);

    retval->edge_count = 0;
    retval->label = nfa_label_obtain();

    dp_memset(retval->edge, 0, sizeof(retval->edge));
    return retval;
}

static inline void
nfa_status_edge_chain(s_fa_status_t *status, char c, s_fa_status_t *next)
{
    uint32 index;

    assert(NULL != next);
    assert(NULL != status);
    assert(NULL == status->edge[status->edge_count]);
    assert(status->edge_count < NFA_EDGE_MAX - 1);

    index = status->edge_count++;

    status->edge[index] = dp_malloc(sizeof(s_fa_edge_t));
    status->edge[index]->c = c;
    status->edge[index]->next = next;
}

static inline s_nfa_t *
nfa_create(void)
{
    s_nfa_t *nfa;

    nfa = dp_malloc(sizeof(s_nfa_t));
    nfa->start = nfa->terminal = NULL;

    return nfa;
}

