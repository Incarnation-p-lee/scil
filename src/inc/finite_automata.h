#ifndef HAVE_DEFINED_FINITE_AUTOMATA_H
#define HAVE_DEFINED_FINITE_AUTOMATA_H

#define NFA_EDGE_MAX           4
#define NULL_CHAR              ((char)0)

typedef struct fa        s_nfa_t;
typedef struct fa        s_dfa_t;
typedef struct fa_status s_fa_status_t;
typedef struct fa_edge   s_fa_edge_t;


struct fa {
    s_fa_status_t *start;
    s_fa_status_t *terminal;
};

struct fa_status {
    uint32       label;
    uint32       edge_count;
    s_fa_edge_t *edge[NFA_EDGE_MAX];
};

struct fa_edge {
    char           c;
    uint32        label;
    s_fa_status_t *next;
};

#endif

