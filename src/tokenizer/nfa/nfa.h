#ifndef HAVE_DEFINED_NFA_H
#define HAVE_DEFINED_NFA_H

#include "finite_automata.h"

#define NFA_LABEL_HASH_SIZE    127
#define RE_EXPAND_MIN        8

struct nfa_edge_map {
    char    c;
    s_nfa_t *nfa;
};
typedef struct nfa_edge_map s_nfa_edge_map_t;

static uint32 nfa_status_lalel = 1;

#if defined DEBUG
    #define NFA_PRINT_BUF_SIZE            1024
    #define NFA_ENGINE_GRAPH_PRINT(nfa)   nfa_engine_graph_print(nfa)
    #define NFA_ENGINE_DESTROY_PRINT(nfa) nfa_engine_destroy_print(nfa)
#else
    #define NFA_ENGINE_GRAPH_PRINT(nfa)
    #define NFA_ENGINE_DESTROY_PRINT(nfa)
#endif

#endif

