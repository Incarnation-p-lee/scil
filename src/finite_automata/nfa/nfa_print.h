#ifndef HAVE_DEFINED_NFA_PRINT_H
#define HAVE_DEFINED_NFA_PRINT_H

#if defined DEBUG
    #define NFA_PRINT_BUF_SIZE            1024
    #define NFA_CLOSURE_PRINT_LEN         10
    #define NFA_ENGINE_GRAPH_PRINT(nfa)   nfa_engine_graph_print(nfa)
    #define NFA_ENGINE_DESTROY_PRINT(nfa) nfa_engine_destroy_print(nfa)
    #define NFA_CLOSURE_PRINT(closure)    nfa_closure_print(closure)
#else
    #define NFA_ENGINE_GRAPH_PRINT(nfa)
    #define NFA_ENGINE_DESTROY_PRINT(nfa)
    #define NFA_CLOSURE_PRINT(closure)
#endif

#endif

