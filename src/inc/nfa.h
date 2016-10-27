#ifndef HAVE_DEFINED_NFA_H
#define HAVE_DEFINED_NFA_H

#define NFA_LABEL_HASH_SIZE    127
#define NFA_SZ_UNMATCH         0
#define NFA_SZ_INVALID         -1
#define NFA_MARK_CHAR          ((char)0xa4)

typedef struct nfa_edge_map    s_nfa_edge_map_t;

struct nfa_edge_map {
    char    c;
    s_nfa_t *nfa;
};

#endif

