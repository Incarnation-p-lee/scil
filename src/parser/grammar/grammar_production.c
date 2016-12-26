static inline s_gr_pdt_t *
grammar_production_create(char *pdt)
{
    char *c;
    s_gr_pdt_t *gr_pdt;

    assert_exit(pdt);

    gr_pdt = dp_malloc(sizeof(*gr_pdt));
    gr_pdt->name = dp_malloc(sizeof(*pdt) * (dp_strlen(pdt) + 1));
    dp_strcpy(gr_pdt->name, pdt);

    gr_pdt->head = grammar_production_head_create(pdt);
    gr_pdt->body_list = grammar_production_body_list_create(pdt);

    return gr_pdt;
}

static inline s_gr_non_tr_t *
grammar_production_head_create(char *pdt)
{
    s_gr_non_tr_t *gr_non_tr;
    char symbol[GR_SYMBOL_NAME_SIZE];

    assert_exit(pdt);

    grammar_symbol_head_obtain(symbol, GR_SYMBOL_NAME_SIZE, pdt);

    gr_non_tr = dp_malloc(sizeof(*gr_non_tr));
    gr_non_tr->non_tr_type = grammar_symbol_non_terminal_obtain(symbol);

    return gr_non_tr;
}

static inline s_gr_pdt_body_list_t *
grammar_production_body_list_create(char *pdt)
{
    char *body;
    uint32 body_cnt;
    s_gr_pdt_body_list_t *body_list;

    assert_exit(pdt);

    body = grammar_symbol_head_skip(pdt);
    body_cnt = grammar_symbol_body_list_count(body);

    body_list = dp_malloc(sizeof(*body_list));
    body_list->index = 0;
}

