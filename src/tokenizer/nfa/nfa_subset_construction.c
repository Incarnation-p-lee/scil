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

    nfa = dp_malloc(sizeof(s_nfa_t));
    nfa->start = status;
    nfa->terminal = next;

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
nfa_subset_rule_induction_binary(s_array_stack_t *stack,  e_regular_meta_opt_t opt)
{
    s_nfa_t *nfa, *nfa_tmp;
    s_nfa_edge_map_t *map, *map_tmp;

    assert_exit(stack);

    map_tmp = array_stack_pop(stack);
    map = array_stack_pop(stack);

    nfa_tmp = nfa_edge_map_nfa_obtain(map_tmp);
    nfa = nfa_edge_map_nfa_obtain(map);

    switch (opt) {
        case RE_M_OPT_AND:
            nfa_subset_rule_induction_and(nfa, nfa_tmp);
            break;
        case RE_M_OPT_OR:
            nfa_subset_rule_induction_or(nfa, nfa_tmp);
            break;
        default:
            assert_exit(false);
            break;
    }

    nfa_edge_map_destroy(map_tmp);
    array_stack_push(stack, map);
    assert_exit(nfa_engine_structure_legal_p(nfa));
}

static inline void
nfa_subset_rule_induction_unary(s_array_stack_t *stack, e_regular_meta_opt_t opt)
{
    s_nfa_t *nfa;
    s_nfa_edge_map_t *map;

    assert_exit(stack);

    map = array_stack_pop(stack);
    nfa = nfa_edge_map_nfa_obtain(map);

    switch (opt) {
        case RE_M_OPT_STAR:
            nfa_subset_rule_induction_star(nfa);
            break;
        case RE_M_OPT_PLUS:
            nfa_subset_rule_induction_plus(nfa);
            break;
        case RE_M_OPT_QUST:
            nfa_subset_rule_induction_question(nfa);
            break;
        default:
            assert_exit(false);
            break;
    }

    array_stack_push(stack, map);
    assert_exit(nfa_engine_structure_legal_p(nfa));
}

/*
 * RE: ab
 */
static inline void
nfa_subset_rule_induction_and(s_nfa_t *s, s_nfa_t *t)
{
    assert_exit(s && t);

    nfa_status_terminal_merge(s->terminal, t->start);
    s->terminal = t->terminal;

    t->start = t->terminal = NULL;
    dp_free(t);
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

        nfa_status_edge_chain(s->terminal, NULL_CHAR, s->start);
        nfa_status_edge_chain(start, NULL_CHAR, terminal);
        nfa_status_edge_chain(start, NULL_CHAR, s->start);
        nfa_status_edge_chain(s->terminal, NULL_CHAR, terminal);

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

        nfa_status_edge_chain(s->terminal, NULL_CHAR, s->start);
        nfa_status_edge_chain(start, NULL_CHAR, s->start);
        nfa_status_edge_chain(s->terminal, NULL_CHAR, terminal);

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

        nfa_status_edge_chain(start, NULL_CHAR, terminal);
        nfa_status_edge_chain(start, NULL_CHAR, s->start);
        nfa_status_edge_chain(s->terminal, NULL_CHAR, terminal);

        s->start = start;
        s->terminal = terminal;
    }
}

