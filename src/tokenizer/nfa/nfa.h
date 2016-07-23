#ifndef HAVE_DEFINED_NFA_H
#define HAVE_DEFINED_NFA_H

#include "finite_automata.h"

#define NFA_LABEL_HASH_SIZE    127

struct nfa_edge_map {
    char    c;
    s_nfa_t *nfa;
};
typedef struct nfa_edge_map s_nfa_edge_map_t;

enum nfa_subset_opt {
    NFA_SUBSET_BKT_L = '(', // left bracket
    NFA_SUBSET_BKT_R = ')', // right bracket
    NFA_SUBSET_AND   = '&',
    NFA_SUBSET_OR    = '|',
    NFA_SUBSET_STAR  = '*',
    NFA_SUBSET_PLUS  = '+',
    NFA_SUBSET_QUST  = '?',
};
typedef enum nfa_subset_opt e_nfa_subset_opt_t;

static uint32 nfa_status_lalel = 1;

#endif

