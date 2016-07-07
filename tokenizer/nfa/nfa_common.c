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

static inline s_nfa_status_t *
nfa_status_create(void)
{
    s_nfa_status_t *retval;

    retval = dp_malloc(sizeof(*retval));
    dp_assert(NULL != retval);

    retval->edge_count = 0;
    retval->label = nfa_label_obtain();

    dp_memset(retval->edge, 0, sizeof(retval->edge));
    return retval;
}

static inline void
nfa_status_edge_chain(s_nfa_status_t *status, char c, s_nfa_status_t *next)
{
    uint32 index;

    dp_assert(NULL != next);
    dp_assert(NULL != status);
    dp_assert(NULL == status->edge[status->edge_count]);
    dp_assert(status->edge_count < NFA_EDGE_MAX - 1);

    index = status->edge_count++;

    status->edge[index] = dp_malloc(sizeof(s_nfa_edge_t));
    status->edge[index]->c = c;
    status->edge[index]->next = next;
}

static inline s_nfa_t *
nfa_create(void)
{
    s_nfa_t *nfa;

    nfa = dp_malloc(sizeof(s_nfa_t));
    nfa->start = nfa->terminal = NULL:
}

