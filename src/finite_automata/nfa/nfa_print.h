#ifndef HAVE_DEFINED_NFA_PRINT_H
#define HAVE_DEFINED_NFA_PRINT_H

#if defined DEBUG
    #define NFA_PRINT_BUF_SIZE                   1024u
    #define NFA_CLOSURE_PRINT_LEN                16u

    #define NFA_ENGINE_GRAPH_PRINT(nfa)          nfa_engine_graph_print(nfa)
    #define NFA_ENGINE_DESTROY_PRINT(nfa)        nfa_engine_destroy_print(nfa)
    #define NFA_CLOSURE_PRINT(closure, c)        nfa_closure_print(closure, c)
    #define NFA_ENGINE_CLOSURE_MATCH_PRINT(n, c) nfa_engine_closure_match_print(n, c)
#else
    #define NFA_ENGINE_GRAPH_PRINT(nfa)
    #define NFA_ENGINE_DESTROY_PRINT(nfa)
    #define NFA_CLOSURE_PRINT(closure, c)
    #define NFA_ENGINE_CLOSURE_MATCH_PRINT(n, c)
#endif

#endif

