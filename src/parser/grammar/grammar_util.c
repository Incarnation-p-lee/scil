static inline uint32
gr_language_pdt_limit(s_gr_lang_t *gr_lang)
{
    assert_exit(gr_language_structure_legal_p(gr_lang));

    return gr_lang->index;
}

static inline bool
gr_pdt_symbol_null_p(s_gr_symbol_t *gr_symbol)
{
    assert_exit(gr_pdt_symbol_structure_legal_p(gr_symbol));

    if (gr_symbol->is_terminal && gr_symbol->terminal.tr_type == GR_TR_NULL) {
        return true;
    } else {
        return false;
    }
}

static inline bool
gr_pdt_body_null_p(s_gr_body_t *gr_body)
{
    assert_exit(gr_pdt_body_structure_legal_p(gr_body));

    if (gr_body->index == 1
        && gr_pdt_symbol_null_p(gr_body->symbol_list[0])) {
        return true;
    } else {
        return false;
    }
}

static inline s_gr_body_list_t *
gr_pdt_body_list(s_gr_pdt_t *gr_pdt)
{
    assert_exit(gr_pdt_structure_legal_p(gr_pdt));

    return gr_pdt->list;
}

static inline s_gr_non_tr_t *
gr_pdt_head(s_gr_pdt_t *gr_pdt)
{
    assert_exit(gr_pdt_structure_legal_p(gr_pdt));

    return gr_pdt->head;
}

static inline e_gr_non_tr_type_t
gr_pdt_head_type(s_gr_pdt_t *gr_pdt)
{
    s_gr_non_tr_t *gr_non_tr;

    assert_exit(gr_pdt_structure_legal_p(gr_pdt));

    gr_non_tr = gr_pdt_head(gr_pdt);

    return gr_non_tr->non_tr_type;
}

static inline s_gr_symbol_t *
gr_pdt_body_symbol(s_gr_body_t *gr_body, uint32 i)
{
    assert_exit(gr_pdt_body_structure_legal_p(gr_body));
    assert_exit(i < gr_pdt_body_symbol_limit(gr_body));

    return gr_body->symbol_list[i];
}

static inline e_gr_non_tr_type_t
gr_pdt_symbol_non_terminal_type(s_gr_symbol_t *gr_symbol)
{
    assert_exit(gr_pdt_symbol_structure_legal_p(gr_symbol));
    assert_exit(!gr_pdt_symbol_terminal_p(gr_symbol));

    return gr_symbol->non_terminal.non_tr_type;
}

static inline bool
gr_pdt_symbol_terminal_p(s_gr_symbol_t *gr_symbol)
{
    assert_exit(gr_pdt_symbol_structure_legal_p(gr_symbol));

    return gr_symbol->is_terminal;
}

static inline uint32
gr_pdt_body_symbol_count(s_gr_body_t *gr_body)
{
    assert_exit(gr_pdt_body_structure_legal_p(gr_body));

    return gr_body->index;
}

static inline uint32
gr_pdt_body_symbol_limit(s_gr_body_t *gr_body)
{
    assert_exit(gr_pdt_body_structure_legal_p(gr_body));

    return gr_pdt_body_symbol_count(gr_body);
}

static inline s_gr_body_t *
gr_pdt_body_list_body(s_gr_body_list_t *gr_body_list, uint32 i)
{
    assert_exit(gr_pdt_body_list_structure_legal_p(gr_body_list));
    assert_exit(i < gr_pdt_body_list_limit(gr_body_list));

    return gr_body_list->body_list[i];
}

static inline uint32
gr_pdt_body_list_limit(s_gr_body_list_t *gr_body_list)
{
    assert_exit(gr_pdt_body_list_structure_legal_p(gr_body_list));

    return gr_body_list->index;
}

static inline bool
gr_pdt_null_contains_p(s_gr_pdt_t *gr_pdt)
{
    uint32 i, limit;
    s_gr_body_t *gr_body;
    s_gr_body_list_t *gr_body_list;

    assert_exit(gr_pdt_structure_legal_p(gr_pdt));

    gr_body_list = gr_pdt->list;

    i = 0;
    limit = gr_pdt_body_list_limit(gr_body_list);

    while (i < limit) {
        gr_body = gr_body_list->body_list[i];

        if (gr_pdt_body_null_p(gr_body)) {
            return true;
        }

        i++;
    }

    return false;
}

static inline s_gr_null_symbol_set_t *
gr_pdt_null_symbol_set_create(void)
{
    s_gr_null_symbol_set_t *gr_null_set;

    gr_null_set = dp_malloc(sizeof(*gr_null_set));

    gr_null_set->queue = array_queue_create();
    gr_null_set->bitmap = bitmap_create(GR_NON_TR_START, GR_NON_TR_LIMIT);

    return gr_null_set;
}

static inline void
gr_pdt_null_symbol_set_destroy(s_gr_null_symbol_set_t *gr_null_set)
{
    assert_exit(gr_pdt_null_symbol_set_structure_legal_p(gr_null_set));

    array_queue_destroy(&gr_null_set->queue);
    bitmap_destroy(&gr_null_set->bitmap);

    dp_free(gr_null_set);
}

static inline uint32
gr_pdt_null_symbol_set_count(s_gr_null_symbol_set_t *gr_null_set)
{
    assert_exit(gr_pdt_null_symbol_set_structure_legal_p(gr_null_set));

    return array_queue_size(gr_null_set->queue);
}

static inline void
gr_pdt_null_symbol_set_add(s_gr_null_symbol_set_t *gr_null_set,
    s_gr_pdt_t *gr_pdt)
{
    native_wide_t symbol_type_value;

    assert_exit(gr_pdt_structure_legal_p(gr_pdt));
    assert_exit(gr_pdt_null_symbol_set_structure_legal_p(gr_null_set));

    symbol_type_value = (native_wide_t)gr_pdt_head_type(gr_pdt);
    assert_exit(bitmap_bit_clear_p(gr_null_set->bitmap, symbol_type_value));

    array_queue_enter(gr_null_set->queue, gr_pdt);
    bitmap_bit_set(gr_null_set->bitmap, symbol_type_value);
}

