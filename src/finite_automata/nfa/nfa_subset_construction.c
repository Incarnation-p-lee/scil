/*
 * Implementation of McMaughton-Yamda-Thompson algorithm
 */

static inline s_nfa_t *
nfa_subset_rule_basic(char c)
{
    s_nfa_t *nfa;
    s_fa_status_t *next;
    s_fa_status_t *status;

    status = nfa_status_create();
    next = nfa_status_create();
    nfa_status_edge_chain(status, c, next);

    nfa = dp_malloc(sizeof(s_nfa_t));
    nfa->start = status;
    nfa->terminal = next;
    nfa->re = NULL;

    return nfa;
}

static inline bool
nfa_status_structure_legal_p(s_fa_status_t *status)
{
    if (!status) {
        return false;
    } else if (status->adj_list && !status->adj_list->succ) {
        return false;
    } else {
        return true;
    }
}

static inline bool
nfa_status_structure_illegal_p(s_fa_status_t *status)
{
    return !nfa_status_structure_legal_p(status);
}

/*
 * RE: a|b
 */
static inline void
nfa_subset_rule_or(s_nfa_t *s, s_nfa_t *t)
{
    assert_exit(nfa_engine_structure_legal_p(s));
    assert_exit(nfa_engine_structure_legal_p(t));

    nfa_status_edge_chain(s->start, NULL_CHAR, t->start);
    nfa_status_edge_chain(t->terminal, NULL_CHAR, s->terminal);

    t->start = t->terminal = NULL;
    nfa_engine_destroy_final(t);
}

static inline void
nfa_subset_rule_binary(s_array_stack_t *stack,  e_regular_wildcard_t opt)
{
    s_nfa_t *nfa, *nfa_tmp;

    assert_exit(stack);

    nfa_tmp = array_stack_pop(stack);
    nfa = array_stack_pop(stack);

    switch (opt) {
        case RE_WILD_AND:
            nfa_subset_rule_and(nfa, nfa_tmp);
            break;
        case RE_WILD_OR:
            nfa_subset_rule_or(nfa, nfa_tmp);
            break;
        default:
            assert_exit(false);
            break;
    }

    array_stack_push(stack, nfa);
    assert_exit(nfa_engine_structure_legal_p(nfa));
}

static inline void
nfa_subset_rule_unary(s_array_stack_t *stack, e_regular_wildcard_t opt)
{
    s_nfa_t *nfa;

    assert_exit(stack);

    nfa = array_stack_pop(stack);

    switch (opt) {
        case RE_WILD_STAR:
            nfa_subset_rule_star(nfa);
            break;
        case RE_WILD_PLUS:
            nfa_subset_rule_plus(nfa);
            break;
        case RE_WILD_QUST:
            nfa_subset_rule_question(nfa);
            break;
        default:
            assert_exit(false);
            break;
    }

    array_stack_push(stack, nfa);
    assert_exit(nfa_engine_structure_legal_p(nfa));
}

/*
 * RE: ab
 */
static inline void
nfa_subset_rule_and(s_nfa_t *s, s_nfa_t *t)
{
    assert_exit(nfa_engine_structure_legal_p(s));
    assert_exit(nfa_engine_structure_legal_p(t));

    nfa_status_terminal_merge(s->terminal, t->start);
    s->terminal = t->terminal;

    t->start = t->terminal = NULL;
    nfa_engine_destroy_final(t);
}

/*
 * RE: a*
 */
static inline void
nfa_subset_rule_star(s_nfa_t *s)
{
    s_fa_status_t *start;
    s_fa_status_t *terminal;

    assert_exit(nfa_engine_structure_legal_p(s));

    start = nfa_status_create();
    terminal = nfa_status_create();

    nfa_status_edge_chain(s->terminal, NULL_CHAR, s->start);
    nfa_status_edge_chain(start, NULL_CHAR, terminal);
    nfa_status_edge_chain(start, NULL_CHAR, s->start);
    nfa_status_edge_chain(s->terminal, NULL_CHAR, terminal);

    s->start = start;
    s->terminal = terminal;
}

/*
 * RE: a+
 */
static inline void
nfa_subset_rule_plus(s_nfa_t *s)
{
    s_fa_status_t *start;
    s_fa_status_t *terminal;

    assert_exit(nfa_engine_structure_legal_p(s));

    start = nfa_status_create();
    terminal = nfa_status_create();

    nfa_status_edge_chain(s->terminal, NULL_CHAR, s->start);
    nfa_status_edge_chain(start, NULL_CHAR, s->start);
    nfa_status_edge_chain(s->terminal, NULL_CHAR, terminal);

    s->start = start;
    s->terminal = terminal;
}

/*
 * RE: a?
 */
static inline void
nfa_subset_rule_question(s_nfa_t *s)
{
    s_fa_status_t *start;
    s_fa_status_t *terminal;

    assert_exit(nfa_engine_structure_legal_p(s));

    start = nfa_status_create();
    terminal = nfa_status_create();

    nfa_status_edge_chain(start, NULL_CHAR, terminal);
    nfa_status_edge_chain(start, NULL_CHAR, s->start);
    nfa_status_edge_chain(s->terminal, NULL_CHAR, terminal);

    s->start = start;
    s->terminal = terminal;
}

