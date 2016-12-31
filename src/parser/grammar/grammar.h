#ifndef HAVE_DEFINED_GRAMMAR_H
#define HAVE_DEFINED_GRAMMAR_H

/*
 * context free grammar
 *
 * Abbreviation Table
 * +--------------------+
 * | gr   | grammar     |
 * +------+-------------+
 * | tr   | terminal    |
 * +------+-------------+
 * | pdt  | production  |
 * +------+-------------+
 * | expr | expression  |
 * +------+-------------+
 */

#define GR_SYMBOL_LIST_MAX                  16u
#define GR_BODY_LIST_MAX                    8u
#define GR_PDT_LIST_MAX                     128u

typedef enum   grammar_symbol_type          e_gr_symbol_type_t;
typedef enum   grammar_non_terminal_type    e_gr_non_tr_type_t;
typedef struct grammar_terminal             s_gr_tr_t;
typedef struct grammar_non_terminal         s_gr_non_tr_t;
typedef struct grammar_symbol               s_gr_symbol_t;
typedef struct grammar_body                 s_gr_body_t;
typedef struct grammar_body_list            s_gr_body_list_t;
typedef struct grammar_production           s_gr_pdt_t;
typedef struct grammar_language             s_gr_lang_t;

enum grammar_symbol_type {
    SYMBOL_TERMINAL,
    SYMBOL_NON_TERMINAL,
    SYMBOL_START,
};

static char *non_tr_type_name[] = {
    "func_decl",     /* 0 */
    "func_link",
    "func_attr",
    "type",
    "type_name",
    "type_ptr",      /* 5 */
    "type_set",
    "type_set_head",
    "type_base",
    "type_int",
    "type_int_attr", /* 10 */
    "type_int_base",
    "type_float",
    "param",
    "param_head",
    "param_tail",    /* 15 */
};

enum grammar_non_terminal_type {
    GR_NON_TR_FUNC_DECL,     /* 0 */
    GR_NON_TR_FUNC_LINK,
    GR_NON_TR_FUNC_ATTR,
    GR_NON_TR_TYPE,
    GR_NON_TR_TYPE_NAME,
    GR_NON_TR_TYPE_PTR,      /* 5 */
    GR_NON_TR_TYPE_SET,
    GR_NON_TR_TYPE_SET_HEAD,
    GR_NON_TR_TYPE_BASE,
    GR_NON_TR_TYPE_INT,
    GR_NON_TR_TYPE_INT_ATTR, /* 10 */
    GR_NON_TR_TYPE_INT_BASE,
    GR_NON_TR_TYPE_FLOAT,
    GR_NON_TR_PARAM,
    GR_NON_TR_PARAM_HEAD,
    GR_NON_TR_PARAM_TAIL,    /* 15 */

    GR_NON_TR_NULL,
    GR_NON_TR_INVALID,
};

struct grammar_terminal {
    e_tk_type_t tk_type;
};

struct grammar_non_terminal {
    e_gr_non_tr_type_t non_tr_type;
};

struct grammar_symbol {
    bool is_terminal;
    union {
        s_gr_tr_t     *terminal;
        s_gr_non_tr_t *non_terminal;
    };
};

struct grammar_body {
    uint32        index;
    s_gr_symbol_t *symbol_list[GR_SYMBOL_LIST_MAX];
};

struct grammar_body_list {
    uint32      index;
    s_gr_body_t *body_list[GR_BODY_LIST_MAX];
};

struct grammar_production {
    char             *name;
    s_gr_non_tr_t    *head;
    s_gr_body_list_t *list;
};

struct grammar_language {
    uint32     size;
    uint32     index;
    s_gr_pdt_t **pdt_list;
};

#endif

