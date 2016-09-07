#ifndef HAVE_DEFINED_FINITE_AUTOMATA_H
#define HAVE_DEFINED_FINITE_AUTOMATA_H

#define NULL_CHAR              ((char)0)
#define STAR_CHAR              '*'
#define SLASH_CHAR             '/'
#define NLINE_CHAR             '\n'
#define SENTINEL_CHAR          ((char)0x20)
#define D_QUOTE_CHAR           ((char)0x22)     // double quote
#define POUND_CHAR             '#'
#define PERCENT_CHAR           '%'
#define SPACE_CHAR             ' '
#define UDRLINE_CHAR           '_'
#define B_SLASH_CHAR           ((char)0x5c)     // back slash
#define MARK_CHAR              ((char)0xa4)

typedef struct fa        s_nfa_t;
typedef struct fa        s_dfa_t;
typedef struct fa_status s_fa_status_t;
typedef struct fa_edge   s_fa_edge_t;

struct fa {
    char          *re;
    s_fa_status_t *start;
    s_fa_status_t *terminal;
};

struct fa_status {
    uint32       label;
    s_fa_edge_t  *adj_list;  // Adjacency list for graph
};

struct fa_edge {
    char                   c;
    uint32                 label;
    s_fa_status_t          *succ;
    s_doubly_linked_list_t list;
};

#endif

