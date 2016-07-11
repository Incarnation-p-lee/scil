//
// Implementation of McMaughton-Yamda-Thompson algorithm
//
static inline s_nfa_t *
nfa_subset_rule_basic(char c)
{
    s_nfa_t *nfa;
    s_fa_status_t *next;
    s_fa_status_t *status;

    status = nfa_status_create();
    next = nfa_status_create();
    nfa_status_edge_chain(status, c, next);

    nfa = nfa_create();
    nfa->start = status;
    nfa->terminal = next;

    return nfa;
}

static inline bool
nfa_status_legal_p(s_fa_status_t *status)
{
    if (!status) {
        return false;
    } else if (status->edge_count >= NFA_EDGE_MAX) {
        return false;
    } else if (status->edge[status->edge_count]) {
        return false;
    } else {
        return true;
    }
}

/*
 * RE: a|b
 */
static inline void
nfa_subset_rule_induction_or(s_nfa_t *s, s_nfa_t *t)
{
    s_fa_status_t *start;
    s_fa_status_t *terminal;

    if (s && t) {
        start = nfa_status_create();
        terminal = nfa_status_create();

        nfa_status_edge_chain(start, NULL_CHAR, s->start);
        nfa_status_edge_chain(start, NULL_CHAR, t->start);

        nfa_status_edge_chain(s->terminal, NULL_CHAR, terminal);
        nfa_status_edge_chain(t->terminal, NULL_CHAR, terminal);

        s->start = start;
        s->terminal = terminal;

        t->start = t->terminal = NULL;
        dp_free(t);
    }
}

static inline void
nfa_subset_rule_induction_binary(s_nfa_t *s, s_nfa_t *t, e_nfa_subset_opt_t opt)
{
    dp_assert(NULL != s);
    dp_assert(NULL != t);

    switch (opt) {
        case NFA_SUBSET_AND:
            nfa_subset_rule_induction_and(s, t);
            break;
        case NFA_SUBSET_OR:
            nfa_subset_rule_induction_or(s, t);
            break;
        default:
            dp_assert(false);
            break;
    }
}

static inline void
nfa_subset_rule_induction_unary(s_nfa_t *s, e_nfa_subset_opt_t opt)
{
    dp_assert(NULL != s);

    switch (opt) {
        case NFA_SUBSET_STAR:
            nfa_subset_rule_induction_star(s);
            break;
        case NFA_SUBSET_PLUS:
            nfa_subset_rule_induction_plus(s);
            break;
        case NFA_SUBSET_QUST:
            nfa_subset_rule_induction_question(s);
            break;
        default:
            dp_assert(false);
            break;
    }
}

/*
 * RE: ab
 */
static inline void
nfa_subset_rule_induction_and(s_nfa_t *s, s_nfa_t *t)
{
    if (s && t) {
        nfa_status_edge_chain(s->terminal, NULL_CHAR, t->start);
        s->terminal = t->terminal;

        t->start = t->terminal = NULL;
        dp_free(t);
    }
}

/*
 * RE: a*
 */
static inline void
nfa_subset_rule_induction_star(s_nfa_t *s)
{
    s_fa_status_t *start;
    s_fa_status_t *terminal;

    if (s) {
        start = nfa_status_create();
        terminal = nfa_status_create();

        nfa_status_edge_chain(start, NULL_CHAR, s->start);
        nfa_status_edge_chain(s->terminal, NULL_CHAR, terminal);
        nfa_status_edge_chain(s->terminal, NULL_CHAR, s->start);
        nfa_status_edge_chain(start, NULL_CHAR, terminal);

        s->start = start;
        s->terminal = terminal;
    }
}

/*
 * RE: a+
 */
static inline void
nfa_subset_rule_induction_plus(s_nfa_t *s)
{
    s_fa_status_t *start;
    s_fa_status_t *terminal;

    if (s) {
        start = nfa_status_create();
        terminal = nfa_status_create();

        nfa_status_edge_chain(start, NULL_CHAR, s->start);
        nfa_status_edge_chain(s->terminal, NULL_CHAR, terminal);
        nfa_status_edge_chain(s->terminal, NULL_CHAR, s->start);

        s->start = start;
        s->terminal = terminal;
    }
}

/*
 * RE: a?
 */
static inline void
nfa_subset_rule_induction_question(s_nfa_t *s)
{
    s_fa_status_t *start;
    s_fa_status_t *terminal;

    if (s) {
        start = nfa_status_create();
        terminal = nfa_status_create();

        nfa_status_edge_chain(start, NULL_CHAR, s->start);
        nfa_status_edge_chain(s->terminal, NULL_CHAR, terminal);
        nfa_status_edge_chain(start, NULL_CHAR, terminal);

        s->start = start;
        s->terminal = terminal;
    }
}

