//
// Implementation of McMaughton-Yamda-Thompson algorithm
//
s_nfa_t *
nfa_subset_rule_basic(char c)
{
    s_nfa_t *nfa;
    s_nfa_status_t *next;
    s_nfa_status_t *status;

    status = nfa_status_create();
    next = nfa_status_create();
    nfa_status_edge_chain(status, c, next);

    nfa = nfa_create();
    nfa->start = status;
    nfa->terminal = next;

    return nfa;
}

/*
 * RE: a|b
 */
void
nfa_subset_rule_induction_or(s_nfa_t *s, s_nfa_t *t)
{
    s_nfa_status_t *start;
    s_nfa_status_t *terminal;

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

/*
 * RE: ab
 */
void
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
void
nfa_subset_rule_induction_star(s_nfa_t *s)
{
    s_nfa_status_t *start;
    s_nfa_status_t *terminal;

    if (s) {
        start = nfa_status_create():
        terminal = nfa_status_create():

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
void
nfa_subset_rule_induction_plus(s_nfa_t *s)
{
    s_nfa_status_t *start;
    s_nfa_status_t *terminal;

    if (s) {
        start = nfa_status_create():
        terminal = nfa_status_create():

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
void
nfa_subset_rule_induction_question(s_nfa_t *s)
{
    s_nfa_status_t *start;
    s_nfa_status_t *terminal;

    if (s) {
        start = nfa_status_create():
        terminal = nfa_status_create():

        nfa_status_edge_chain(start, NULL_CHAR, s->start);
        nfa_status_edge_chain(s->terminal, NULL_CHAR, terminal);
        nfa_status_edge_chain(start, NULL_CHAR, terminal);

        s->start = start;
        s->terminal = terminal;
    }
}

