#ifndef HAVE_DEFINED_FINITE_AUTOMATA_H
#define HAVE_DEFINED_FINITE_AUTOMATA_H

typedef struct fa              s_nfa_t;
typedef struct fa              s_dfa_t;
typedef struct fa_status       s_fa_status_t;
typedef struct fa_edge         s_fa_edge_t;
typedef struct range_uint32    s_range_uint32_t;
typedef struct fa_closure      s_fa_closure_t;
typedef struct fa_closure_dp   s_fa_closure_dp_t;

struct range_uint32 {
    uint32 min;
    uint32 max;
};

struct fa {
    char             *re;
    s_range_uint32_t label_range;
    s_fa_status_t    *start;
    s_fa_status_t    *terminal;
};

struct fa_status {
    uint32       label;
    s_fa_edge_t  *adj_list;       // Adjacency list for graph
};

struct fa_edge {
    char                   c;
    uint32                 label; // The label of successor status
    s_fa_status_t          *succ;
    s_doubly_linked_list_t list;
};

struct fa_closure {
    char            c;
    s_array_queue_t *path_queue;
    s_array_queue_t *collection;
    s_bitmap_t      *bitmap;
};

/*
 * dp means dynamic programming here.
 */
struct fa_closure_dp {
    s_fa_closure_t **dp;
    uint32         size;
    uint32         index;
};

#endif

