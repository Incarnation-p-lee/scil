static inline s_gr_pdt_t *
gr_pdt_create(char *pdt_name)
{
    char *body_list_name;
    s_gr_pdt_t *pdt;

    assert_exit(pdt_name);

    body_list_name = gr_string_head_skip(pdt_name);

    pdt = dp_malloc(sizeof(*pdt));
    pdt->name = dp_malloc(sizeof(*pdt_name) * (dp_strlen(pdt_name) + 1));
    dp_strcpy(pdt->name, pdt_name);

    pdt->head = gr_pdt_head_create(pdt_name);
    pdt->list = gr_body_list_create(body_list_name);

    return pdt;
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
gr_pdt_head_create(char *pdt_name)
{
    s_gr_non_tr_t *non_tr;
    char symbol[GR_STR_NAME_SIZE];

    assert_exit(pdt_name);

    gr_string_head_obtain(symbol, GR_STR_NAME_SIZE, pdt_name);

    non_tr = dp_malloc(sizeof(*non_tr));
    non_tr->type = gr_string_non_terminal_obtain(symbol);

    return non_tr;
}

static inline void
gr_pdt_head_destroy(s_gr_non_tr_t *non_tr)
{
    assert_exit(gr_non_terminal_structure_legal_p(non_tr));

    dp_free(non_tr);
}

static inline s_gr_body_list_t *
gr_body_list_create(char *body_list_name)
{
    char *c;
    s_gr_body_t *body;
    s_gr_body_list_t *body_list;
    char body_name[GR_STR_BODY_SIZE];

    assert_exit(body_list_name);

    body_list = dp_malloc(sizeof(*body_list));
    body_list->index = 0;
    c = body_list_name;

    while (*c) {
        c += gr_string_body_fill(body_name, GR_STR_BODY_SIZE, c);

        body = gr_body_create(body_name);
        body_list->body_list[body_list->index++] = body;

        assert_exit(body_list->index <= GR_BODY_LIST_MAX);
    }

    return body_list;
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
 * body_name can be null string here.
 */
static inline s_gr_body_t *
gr_body_create(char *body_name)
{
    char *c;
    s_gr_body_t *body;
    s_gr_symbol_t *symbol;
    char symbol_name[GR_STR_NAME_SIZE];

    assert_exit(body_name);

    body = dp_malloc(sizeof(*body));
    body->index = 0;
    c = body_name;

    while (*c) {
        c += gr_string_symbol_fill(symbol_name, GR_STR_NAME_SIZE, c);

        symbol = gr_symbol_create(symbol_name);

        gr_body_symbol_append(body, symbol);
    }

    return body;
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
gr_symbol_create(char *symbol_name)
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

static inline s_gr_null_pdt_helper_t *
gr_null_pdt_helper_create(void)
{
    s_gr_null_pdt_helper_t *null_pdt_helper;

    null_pdt_helper = dp_malloc(sizeof(*null_pdt_helper));

    null_pdt_helper->i = null_pdt_helper->s = 0;
    null_pdt_helper->pdt_queue = array_queue_create();
    null_pdt_helper->null_pdt_queue = array_queue_create();
    null_pdt_helper->null_pdt_bitmap = bitmap_create(GR_NON_TR_START, GR_NON_TR_LIMIT);

    return null_pdt_helper;
}

static inline void
gr_null_pdt_helper_destroy(s_gr_null_pdt_helper_t *null_pdt_helper)
{
    assert_exit(gr_null_pdt_helper_structure_legal_p(null_pdt_helper));

    array_queue_destroy(&null_pdt_helper->null_pdt_queue);
    bitmap_destroy(&null_pdt_helper->null_pdt_bitmap);
    array_queue_destroy(&null_pdt_helper->pdt_queue);

    dp_free(null_pdt_helper);
}

