s_nfa_t *
nfa_engine_create(char *re)
{
    char *c;
    s_array_stack_t *stack;
    s_nfa_t *nfa, *nfa_tmp;
    s_nfa_edge_map_t *map, *map_tmp;

    nfa = NULL;
    // re should be Reverse Polish Expression
    // Add checking here.
    if (re) {
        c = re;
        stack = array_stack_create();

        while (*c) {
            if (dp_isalnum(*c) || '_' == *c) {
                map = nfa_edge_map_create(*c, NULL);
                array_stack_push(stack, map);
            } else {
                switch (*c) {
                    case NFA_SUBSET_OR:
                    case NFA_SUBSET_AND:
                        map = array_stack_pop(stack);
                        map_tmp = array_stack_pop(stack);
                        nfa = nfa_edge_map_nfa_obtain(map);
                        nfa_tmp = nfa_edge_map_nfa_obtain(map_tmp);

                        nfa_subset_rule_induction_binary(nfa, nfa_tmp, *c);
                        nfa_edge_map_destroy(map_tmp);
                        array_stack_push(stack, map);
                        break;
                    case NFA_SUBSET_STAR:
                    case NFA_SUBSET_PLUS:
                    case NFA_SUBSET_QUST:
                        map = array_stack_pop(stack);
                        nfa = nfa_edge_map_nfa_obtain(map);

                        nfa_subset_rule_induction_unary(nfa, *c);
                        array_stack_push(stack, map);
                        break;
                    default:
                        dp_assert(false);
                        break;
                }
            }
            c++;
        }

        map = array_stack_pop(stack);
        dp_assert(array_stack_empty_p(stack));

        nfa = map->nfa;
        map->nfa = NULL;
        nfa_edge_map_destroy(map);
        array_stack_destroy(&stack);
    }

    return nfa;
}


