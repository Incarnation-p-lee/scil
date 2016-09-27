#ifndef HAVE_DEFINED_NFA_H
#define HAVE_DEFINED_NFA_H

#define NFA_LABEL_HASH_SIZE    127
#define RE_EXPAND_MIN          8
#define SZ_UNMATCH             -1

typedef struct nfa_edge_map    s_nfa_edge_map_t;

struct nfa_edge_map {
    char    c;
    s_nfa_t *nfa;
};

#endif

