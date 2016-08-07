#ifndef HAVE_DEFINED_NFA_H
#define HAVE_DEFINED_NFA_H

#include "finite_automata.h"

#define NFA_LABEL_HASH_SIZE    127
#define NFA_RE_SIZE_MIN        8

struct nfa_edge_map {
    char    c;
    s_nfa_t *nfa;
};
typedef struct nfa_edge_map s_nfa_edge_map_t;

enum nfa_subset_opt {
    NFA_SUBSET_BKT_L  = '(', // left bracket
    NFA_SUBSET_BKT_R  = ')', // right bracket
    NFA_SUBSET_AND    = '&',
    NFA_SUBSET_OR     = '|',
    NFA_SUBSET_STAR   = '*',
    NFA_SUBSET_PLUS   = '+',
    NFA_SUBSET_QUST   = '?',
    NFA_SUBSET_BKT_LM = '[',
    NFA_SUBSET_BKT_RM = ']',
    NFA_SUBSET_CNNT   = '-',
};
typedef enum nfa_subset_opt e_nfa_subset_opt_t;

static uint32 nfa_status_lalel = 1;

#if defined DEBUG
    #define NFA_PRINT_BUF_SIZE     1024
    #define NFA_ENGINE_GRAPH_PRINT(nfa)  nfa_engine_graph_print(nfa)
#else
    #define NFA_ENGINE_GRAPH_PRINT(nfa)
#endif

#endif

