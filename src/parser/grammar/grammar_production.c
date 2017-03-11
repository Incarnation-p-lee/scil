static inline s_gr_pdt_t *
gr_pdt_create(char *pdt)
{
    char *body_list;
    s_gr_pdt_t *gr_pdt;

    assert_exit(pdt);

    body_list = gr_string_head_skip(pdt);

    gr_pdt = dp_malloc(sizeof(*gr_pdt));
    gr_pdt->name = dp_malloc(sizeof(*pdt) * (dp_strlen(pdt) + 1));
    dp_strcpy(gr_pdt->name, pdt);

    gr_pdt->head = gr_pdt_head_create(pdt);
    gr_pdt->list = gr_pdt_body_list_create(body_list);

    return gr_pdt;
}

static inline void
gr_pdt_destroy(s_gr_pdt_t *pdt)
{
    assert_exit(gr_pdt_structure_legal_p(pdt));

    gr_pdt_head_destroy(pdt->head);
    gr_body_list_destroy(pdt->list);

    dp_free(pdt->name);
    dp_free(pdt);
}

static inline s_gr_non_tr_t *
gr_pdt_head_create(char *pdt)
{
    s_gr_non_tr_t *gr_non_tr;
    char symbol[GR_STR_NAME_SIZE];

    assert_exit(pdt);

    gr_string_head_obtain(symbol, GR_STR_NAME_SIZE, pdt);

    gr_non_tr = dp_malloc(sizeof(*gr_non_tr));
    gr_non_tr->type = gr_string_non_terminal_obtain(symbol);

    return gr_non_tr;
}

static inline void
gr_pdt_head_destroy(s_gr_non_tr_t *gr_non_tr)
{
    assert_exit(gr_non_terminal_structure_legal_p(gr_non_tr));

    dp_free(gr_non_tr);
}

static inline s_gr_body_list_t *
gr_pdt_body_list_create(char *body_list)
{
    char *c;
    s_gr_body_t *gr_body;
    s_gr_body_list_t *gr_body_list;
    char body[GR_STR_BODY_SIZE];

    assert_exit(body_list);

    gr_body_list = dp_malloc(sizeof(*gr_body_list));
    gr_body_list->index = 0;
    c = body_list;

    while (*c) {
        c += gr_string_body_fill(body, GR_STR_BODY_SIZE, c);

        gr_body = gr_pdt_body_create(body);
        gr_body_list->body_list[gr_body_list->index++] = gr_body;

        assert_exit(gr_body_list->index <= GR_BODY_LIST_MAX);
    }

    return gr_body_list;
}

static inline void
gr_body_list_destroy(s_gr_body_list_t *body_list)
{
    uint32 i;
    uint32 limit;

    assert_exit(gr_pdt_body_list_structure_legal_p(body_list));

    i = 0;
    limit = body_list->index;

    while (i < limit) {
        gr_body_destroy(body_list->body_list[i]);
        i++;
    }

    dp_free(body_list);
}

/*
 * body can be null string here.
 */
static inline s_gr_body_t *
gr_pdt_body_create(char *body)
{
    char *c;
    s_gr_body_t *gr_body;
    s_gr_symbol_t *symbol;
    char symbol_name[GR_STR_NAME_SIZE];

    assert_exit(body);

    gr_body = dp_malloc(sizeof(*gr_body));
    gr_body->index = 0;
    c = body;

    while (*c) {
        c += gr_string_symbol_fill(symbol_name, GR_STR_NAME_SIZE, c);

        symbol = gr_pdt_symbol_create(symbol_name);

        gr_body_symbol_append(gr_body, symbol);
    }

    return gr_body;
}

static inline void
gr_body_destroy(s_gr_body_t *body)
{
    uint32 i;
    uint32 limit;

    assert_exit(gr_pdt_body_structure_legal_p(body));

    i = 0;
    limit = body->index;

    while (i < limit) {
        gr_symbol_destroy(body->symbol_list[i]);
        i++;
    }

    dp_free(body);
}

static inline s_gr_symbol_t *
gr_pdt_symbol_create(char *symbol_name)
{
    s_gr_symbol_t *symbol;
    e_gr_tr_type_t tr_type;
    e_gr_non_tr_type_t non_tr_type;

    assert_exit(symbol_name);

    symbol = dp_malloc(sizeof(*symbol));
    non_tr_type = gr_string_non_terminal_obtain(symbol_name);
    tr_type = gr_string_terminal_obtain(symbol_name);

    if (non_tr_type != GR_NON_TR_INVALID) {
        symbol->is_terminal = false;
        symbol->non_terminal.type = non_tr_type;
    } else if (tr_type != GR_TR_INVALID) {
        symbol->is_terminal = true;
        symbol->terminal.type = tr_type;
    } else {
        log_print_and_exit("Unknown type of symbol '%s'.\n", symbol_name);
    }

    return symbol;
}

static inline void
gr_symbol_destroy(s_gr_symbol_t *symbol)
{
    assert_exit(gr_pdt_symbol_structure_legal_p(symbol));

    dp_free(symbol);
}

