static inline s_gr_pdt_t *
grammar_production_create(char *pdt)
{
    s_gr_pdt_t *gr_pdt;

    assert_exit(pdt);

    gr_pdt = dp_malloc(sizeof(*gr_pdt));
    gr_pdt->name = dp_malloc(sizeof(*pdt) * (dp_strlen(pdt) + 1));
    dp_strcpy(gr_pdt->name, pdt);

    gr_pdt->head = grammar_production_head_create(pdt);
    gr_pdt->list = grammar_production_body_list_create(pdt);

    return gr_pdt;
}

static inline s_gr_non_tr_t *
grammar_production_head_create(char *pdt)
{
    s_gr_non_tr_t *gr_non_tr;
    char symbol[GR_STR_NAME_SIZE];

    assert_exit(pdt);

    grammar_string_head_obtain(symbol, GR_STR_NAME_SIZE, pdt);

    gr_non_tr = dp_malloc(sizeof(*gr_non_tr));
    gr_non_tr->non_tr_type = grammar_string_non_terminal_obtain(symbol);

    return gr_non_tr;
}

static inline s_gr_body_list_t *
grammar_production_body_list_create(char *body_list)
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
        c += grammar_string_body_fill(body, GR_STR_BODY_SIZE, c);

        gr_body = grammar_production_body_create(body);
        gr_body_list->body_list[gr_body_list->index++] = gr_body;

        assert_exit(gr_body_list->index <= GR_BODY_LIST_MAX);
    }

    return gr_body_list;
}

static inline s_gr_body_t *
grammar_production_body_create(char *body)
{
    char *c;
    s_gr_body_t *gr_body;
    s_gr_symbol_t *gr_symbol;
    char symbol[GR_STR_NAME_SIZE];

    assert_exit(body);

    gr_body = dp_malloc(sizeof(*gr_body));
    gr_body->index = 0;
    c = body;

    while (*c) {
        c += grammar_string_symbol_fill(symbol, GR_STR_NAME_SIZE, c);

        gr_symbol = grammar_production_symbol_create(symbol);
        gr_body->symbol_list[gr_body->index++] = gr_symbol;

        assert_exit(gr_body->index <= GR_SYMBOL_LIST_MAX);
    }

    return gr_body;
}

static inline s_gr_symbol_t *
grammar_production_symbol_create(char *symbol)
{
    s_gr_symbol_t *gr_symbol;
    e_gr_non_tr_type_t non_tr_type;

    assert_exit(symbol);

    gr_symbol = dp_malloc(sizeof(*gr_symbol));
    non_tr_type = grammar_string_non_terminal_obtain(symbol);

    if (non_tr_type == GR_NON_TR_INVALID) {
        gr_symbol->is_terminal = true;
        // To-be-continue
    } else {
        gr_symbol->is_terminal = false;
        // To-be-continue
    }

    return gr_symbol;
}


