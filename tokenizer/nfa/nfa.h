#ifndef HAVE_DEFINED_NFA_H
#define HAVE_DEFINED_NFA_H


#define NFA_EDGE_MAX           4
#define NULL_CHAR              ((char)0)

typedef struct nfa        s_nfa_t;
typedef struct nfa_status s_nfa_status_t;
typedef struct nfa_edge   s_nfa_edge_t;

struct nfa {
    s_nfa_status_t *start;
    s_nfa_status_t *terminal;
};

struct nfa_status {
    uint32       label;
    uint32       edge_count;
    s_nfa_edge_t *edge[NFA_EDGE_MAX];
};

struct nfa_edge {
    char           c;
    s_nfa_status_t *next;
};


static uint32 nfa_status_lalel = 0;

#endif

