static inline void
gr_transform_null_pdt_seek_init(s_gr_null_symbol_set_t *gr_null_set,
    s_array_queue_t *pdt_queue, s_gr_lang_t *gr_lang)
{
    uint32 i, limit;
    s_gr_pdt_t *gr_pdt;

    assert_exit(gr_language_structure_legal_p(gr_lang));
    assert_exit(array_queue_structure_legal_p(pdt_queue));
    assert_exit(gr_pdt_null_symbol_set_structure_legal_p(gr_null_set));

    i = 0;
    limit = gr_language_pdt_limit(gr_lang);

    while (i < limit) {
        gr_pdt = gr_lang->pdt_list[i];

        if (gr_pdt_null_contains_p(gr_pdt)) {
            gr_pdt_null_symbol_set_add(gr_null_set, gr_pdt);
        } else {
            array_queue_enter(pdt_queue, gr_pdt);
        }

        i++;
    }
}

static inline bool
gr_transform_null_body_inferred_p(s_gr_null_symbol_set_t *gr_null_set,
    s_gr_body_t *gr_body)
{
    uint32 i, limit;
    s_gr_symbol_t *gr_symbol;
    e_gr_non_tr_type_t type;

    assert_exit(gr_pdt_body_structure_legal_p(gr_body));
    assert_exit(gr_pdt_null_symbol_set_structure_legal_p(gr_null_set));

    i = 0;
    limit = gr_pdt_body_symbol_limit(gr_body);

    while (i < limit) {
        gr_symbol = gr_pdt_body_symbol(gr_body, i);
        type = gr_pdt_symbol_non_terminal_type(gr_symbol);

        if (gr_pdt_symbol_terminal_p(gr_symbol)) {
            return false;
        } else if (bitmap_bit_clear_p(gr_null_set->bitmap, type)) {
            return false;
        }

        i++;
    }

    return true;
}

static inline bool
gr_transform_null_pdt_inferred_p(s_gr_null_symbol_set_t *gr_null_set,
    s_gr_pdt_t *gr_pdt)
{
    uint32 i, limit;
    s_gr_body_t *gr_body;
    s_gr_body_list_t *gr_body_list;

    assert_exit(gr_pdt_structure_legal_p(gr_pdt));
    assert_exit(gr_pdt_null_symbol_set_structure_legal_p(gr_null_set));

    gr_body_list = gr_pdt_body_list(gr_pdt);

    i = 0;
    limit = gr_pdt_body_list_limit(gr_body_list);

    while (i < limit) {
        gr_body = gr_pdt_body_list_body(gr_body_list, i);

        if (gr_transform_null_body_inferred_p(gr_null_set, gr_body)) {
            return true;
        }

        i++;
    }

    return false;
}

static inline void
gr_transform_null_pdt_seek(s_gr_null_symbol_set_t *gr_null_set,
    s_gr_lang_t *gr_lang)
{
    s_gr_pdt_t *gr_pdt;
    uint32 null_pdt_count;
    s_array_queue_t *pdt_queue_tmp;

    assert_exit(gr_language_structure_legal_p(gr_lang));
    assert_exit(gr_pdt_null_symbol_set_structure_legal_p(gr_null_set));

    null_pdt_count = 0;
    pdt_queue_tmp = array_queue_create();

    gr_transform_null_pdt_seek_init(gr_null_set, pdt_queue_tmp, gr_lang);

    while (null_pdt_count != gr_pdt_null_symbol_set_count(gr_null_set)) {
        null_pdt_count = gr_pdt_null_symbol_set_count(gr_null_set);
        gr_pdt = array_queue_leave(pdt_queue_tmp);

        if (gr_transform_null_pdt_inferred_p(gr_null_set, gr_pdt)) {
            gr_pdt_null_symbol_set_add(gr_null_set, gr_pdt);
        } else {
            array_queue_enter(pdt_queue_tmp, gr_pdt);
        }
    }

    array_queue_destroy(&pdt_queue_tmp);
    GR_NULL_PDT_PRINT(gr_null_set);
}

static inline void
gr_transform_null_pdt_eliminate(s_gr_lang_t *gr_lang)
{
    s_gr_null_symbol_set_t *gr_null_set;

    assert_exit(gr_language_structure_legal_p(gr_lang));

    gr_null_set = gr_pdt_null_symbol_set_create();

    gr_transform_null_pdt_seek(gr_null_set, gr_lang);

    gr_pdt_null_symbol_set_destroy(gr_null_set);
}


