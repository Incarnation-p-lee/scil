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

#define GR_SYMBOL_LIST_MAX                  64u
#define GR_BODY_LIST_MAX                    64u
#define GR_PDT_LIST_MAX                     128u

typedef enum   grammar_symbol_type          e_gr_symbol_type_t;
typedef enum   grammar_terminal_type        e_gr_tr_type_t;
typedef enum   grammar_non_terminal_type    e_gr_non_tr_type_t;
typedef enum   grammar_symbol_state         e_gr_symbol_state_t;

typedef struct grammar_terminal             s_gr_tr_t;
typedef struct grammar_non_terminal         s_gr_non_tr_t;
typedef struct grammar_symbol               s_gr_symbol_t;
typedef struct grammar_body                 s_gr_body_t;
typedef struct grammar_body_list            s_gr_body_list_t;
typedef struct grammar_production           s_gr_pdt_t;
typedef struct grammar_language             s_gr_lang_t;
typedef struct grammar_null_pdt_helper      s_gr_null_pdt_helper_t;

enum grammar_symbol_state {
    GR_SYMBOL_STATE_OFF = 0u,
    GR_SYMBOL_STATE_ON  = 1u,
    GR_SYMBOL_STATE_INVALID,
};

enum grammar_symbol_type {
    SYMBOL_TERMINAL,
    SYMBOL_NON_TERMINAL,
    SYMBOL_START,
};

/* *HACK* do NOT change the order of following type and name */
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
    "func_impl",
    "func_body",
    "{",
    "}",

    "non_tr_tmp_0",
    "non_tr_tmp_1",
    "non_tr_tmp_2",
};

enum grammar_non_terminal_type {
    GR_NON_TR_FUNC_DECL = 0, /* 0 */
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
    GR_NON_TR_FUNC_IMPL,
    GR_NON_TR_FUNC_BODY,
    GR_NON_TR_BLOCK_LEFT,
    GR_NON_TR_BLOCK_RIGHT,

    GR_NON_TR_TMP_0,
    GR_NON_TR_TMP_1,
    GR_NON_TR_TMP_2,

    GR_NON_TR_TMP_LIMIT,
    GR_NON_TR_LIMIT,
    GR_NON_TR_INVALID,

    GR_NON_TR_TMP_START = GR_NON_TR_TMP_0,
    GR_NON_TR_START = GR_NON_TR_FUNC_DECL,
};

// static e_gr_non_tr_type_t gr_symbol_temp_base = GR_NON_TR_TMP_START;

static char *tr_type_name[] = {
    "id",       /* 0 */
    "(",
    ")",
    ";",
    ",",

    "static",   /* 5 */
    "extern",
    "inline",
    "void",
    "struct",
    "union",    /* 10 */
    "enum",
    "unsigned",
    "signed",
    "char",
    "int",      /* 15 */
    "short",
    "long",
    "float",
    "double",

    "*",        /* 20 */

    "$"
};

enum grammar_terminal_type {
    GR_TR_IDTR,     /* 0 */
    GR_TR_L_S_BRKT,
    GR_TR_R_S_BRKT,
    GR_TR_SEMI,
    GR_TR_COMMA,

    GR_TR_STATIC,   /* 5 */
    GR_TR_EXTERN,
    GR_TR_INLINE,
    GR_TR_VOID,
    GR_TR_STRUCT,
    GR_TR_UNION,    /* 10 */
    GR_TR_ENUM,
    GR_TR_UNSIGNED,
    GR_TR_SIGNED,
    GR_TR_CHAR,
    GR_TR_INT,      /* 15 */
    GR_TR_SHORT,
    GR_TR_LONG,
    GR_TR_FLOAT,
    GR_TR_DOUBLE,

    GR_TR_STAR,     /* 20 */

    GR_TR_NULL,     /* $ */

    GR_TR_INVALID,
};

struct grammar_terminal {
    e_gr_tr_type_t type;
};

struct grammar_non_terminal {
    e_gr_non_tr_type_t type;
};

struct grammar_symbol {
    bool is_terminal;
    union {
        s_gr_tr_t     terminal;
        s_gr_non_tr_t non_terminal;
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
    void        *data;
    e_gr_type_t gr_type;
    uint32      size;
    uint32      index;
    s_gr_pdt_t  **pdt_list;
};

struct grammar_null_pdt_helper {
    s_array_queue_t     *pdt_queue;
    s_array_queue_t     *null_pdt_queue;
    s_bitmap_t          *null_pdt_bitmap;

    uint32              i; /* for index list */
    uint32              index_list[GR_SYMBOL_LIST_MAX];
    uint32              s; /* for state list */
    e_gr_symbol_state_t state_list[GR_SYMBOL_LIST_MAX];
};

#endif

