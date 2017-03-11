static inline void
gr_null_pdt_helper_initial(s_gr_null_pdt_helper_t *null_pdt_helper,
    s_gr_lang_t *gr_lang)
{
    uint32 i, limit;
    s_gr_pdt_t *pdt;

    assert_exit(gr_language_structure_legal_p(gr_lang));
    assert_exit(gr_null_pdt_helper_structure_legal_p(null_pdt_helper));

    i = 0;
    limit = gr_language_pdt_limit(gr_lang);

    while (i < limit) {
        pdt = gr_lang->pdt_list[i];

        if (gr_pdt_null_body_contains_p(pdt)) {
            gr_null_pdt_helper_null_pdt_add(null_pdt_helper, pdt);
        } else {
            gr_null_pdt_helper_pdt_add(null_pdt_helper, pdt);
        }

        i++;
    }
}

static inline bool
gr_null_body_inferred_p(s_gr_null_pdt_helper_t *null_pdt_helper, s_gr_body_t *body)
{
    uint32 i, limit;
    s_gr_symbol_t *symbol;
    e_gr_non_tr_type_t type;
    s_bitmap_t *null_pdt_bitmap;

    assert_exit(gr_pdt_body_structure_legal_p(body));
    assert_exit(gr_null_pdt_helper_structure_legal_p(null_pdt_helper));

    i = 0;
    limit = gr_body_symbol_limit(body);
    null_pdt_bitmap = gr_null_pdt_helper_null_pdt_bitmap(null_pdt_helper);

    while (i < limit) {
        symbol = gr_body_symbol(body, i);

        if (gr_pdt_symbol_terminal_p(symbol)) {
            return false;
        }

        type = gr_symbol_non_terminal_type(symbol);

        if (bitmap_bit_clear_p(null_pdt_bitmap, type)) {
            return false;
        }

        i++;
    }

    /* all symbol in body inferred null */
    return true;
}

static inline bool
gr_null_pdt_helper_null_inferred_p(s_gr_null_pdt_helper_t *null_pdt_helper,
    s_gr_pdt_t *pdt)
{
    uint32 i, limit;
    s_gr_body_t *body;
    s_gr_body_list_t *body_list;

    assert_exit(gr_pdt_structure_legal_p(pdt));
    assert_exit(gr_null_pdt_helper_structure_legal_p(null_pdt_helper));

    body_list = gr_pdt_body_list(pdt);

    i = 0;
    limit = gr_body_list_body_limit(body_list);

    while (i < limit) {
        body = gr_body_list_body(body_list, i);

        if (gr_null_body_inferred_p(null_pdt_helper, body)) {
            return true;
        }

        i++;
    }

    return false;
}

static inline void
gr_null_pdt_helper_isolate(s_gr_null_pdt_helper_t *null_pdt_helper)
{
    s_gr_pdt_t *pdt;
    uint32 null_count;
    s_array_queue_t *pdt_queue;
    s_array_queue_t *pdt_queue_tmp;

    assert_exit(gr_null_pdt_helper_structure_legal_p(null_pdt_helper));

    null_count = 0;
    pdt_queue_tmp = array_queue_create();
    pdt_queue = gr_null_pdt_helper_pdt_queue(null_pdt_helper);

    /* if null set pdt unchanged, stable */
    while (null_count != gr_null_pdt_helper_null_pdt_count(null_pdt_helper)) {
        while (!array_queue_empty_p(pdt_queue)) {
            pdt = array_queue_leave(pdt_queue);

            if (gr_null_pdt_helper_null_inferred_p(null_pdt_helper, pdt)) {
                gr_null_pdt_helper_null_pdt_add(null_pdt_helper, pdt);
            } else {
                array_queue_enter(pdt_queue_tmp, pdt);
            }
        }

        array_queue_copy(pdt_queue, pdt_queue_tmp);
        array_queue_cleanup(pdt_queue_tmp);

        null_count = gr_null_pdt_helper_null_pdt_count(null_pdt_helper);
    }

    array_queue_destroy(&pdt_queue_tmp);
    GR_NULL_PDT_HELPER_PRINT(null_pdt_helper);
}

static inline void
gr_null_pdt_helper_null_symbol_seek(s_gr_null_pdt_helper_t *null_pdt_helper,
    s_gr_body_t *body)
{
    uint32 i, limit;
    s_gr_symbol_t *symbol;
    e_gr_non_tr_type_t type;
    s_bitmap_t *null_pdt_bitmap;

    assert_exit(gr_pdt_body_structure_legal_p(body));
    assert_exit(gr_null_pdt_helper_structure_legal_p(null_pdt_helper));

    null_pdt_bitmap = gr_null_pdt_helper_null_pdt_bitmap(null_pdt_helper);
    gr_null_pdt_helper_index_cleanup(null_pdt_helper);

    i = 0;
    limit = gr_body_symbol_limit(body);

    while (i < limit) {
        symbol = gr_body_symbol(body, i);

        if (gr_symbol_non_terminal_p(symbol)) {
            type = gr_symbol_non_terminal_type(symbol);

            if (bitmap_bit_set_p(null_pdt_bitmap, type)) { /* null inferred */
                gr_null_pdt_helper_index_list_append(null_pdt_helper, i);
            }
        }

        i++;
    }
}

static inline void
gr_null_pdt_helper_state_update(s_gr_null_pdt_helper_t *null_pdt_helper,
    uint64 state_mask)
{
    uint32 i;
    e_gr_symbol_state_t state;
    uint32 index_size, index_last;

    assert_exit(gr_null_pdt_helper_structure_legal_p(null_pdt_helper));

    index_size = gr_null_pdt_helper_index_list_size(null_pdt_helper);
    index_last = index_size - 1;

    i = 0;
    gr_null_pdt_helper_state_cleanup(null_pdt_helper);

    while (i < index_size) {
        state = (e_gr_symbol_state_t)BIT_64_GET(state_mask, index_last - i);
        gr_null_pdt_helper_state_list_append(null_pdt_helper, state);

        i++;
    }
}

static inline s_gr_body_t *
gr_null_pdt_helper_body_copy(s_gr_null_pdt_helper_t *null_pdt_helper,
    s_gr_body_t *body)
{
    uint32 i, limit;
    uint32 s, s_limit;
    s_gr_body_t *copy_body;

    assert_exit(gr_pdt_body_structure_legal_p(body));
    assert_exit(gr_null_pdt_helper_structure_legal_p(null_pdt_helper));

    s = i = 0;
    limit = gr_body_symbol_limit(body);
    s_limit = gr_null_pdt_helper_state_list_size(null_pdt_helper);
    copy_body = gr_body_create("");

    while (i < limit) {
        if (s < s_limit && gr_null_pdt_helper_index(null_pdt_helper, s) == i) {
            if (gr_null_pdt_helper_state_on_p(null_pdt_helper, s)) {
                gr_body_symbol_append(copy_body, gr_body_symbol_copy(body, i));
            }

            s++;
        } else {
            gr_body_symbol_append(copy_body, gr_body_symbol_copy(body, i));
        }

        i++;
    }

    return copy_body;
}

static inline void
gr_null_pdt_helper_null_symbol_extend(s_gr_null_pdt_helper_t *null_pdt_helper,
    s_gr_body_list_t *body_list, s_gr_body_t *body)
{
    uint64 size;
    uint64 state_mask, limit;
    s_gr_body_t *body_copy;

    assert_exit(gr_pdt_body_structure_legal_p(body));
    assert_exit(gr_pdt_body_list_structure_legal_p(body_list));
    assert_exit(gr_null_pdt_helper_structure_legal_p(null_pdt_helper));

    state_mask = 0;
    size = gr_null_pdt_helper_index_list_size(null_pdt_helper);
    limit = (1 << size) - 1; /* skip all 1 mask pdt */

    while (state_mask < limit) {
        gr_null_pdt_helper_state_update(null_pdt_helper, state_mask);
        body_copy = gr_null_pdt_helper_body_copy(null_pdt_helper, body);

        if (gr_body_empty_p(body_copy)) {
            gr_body_destroy(body_copy);
        } else {
            gr_body_list_body_append(body_list, body_copy);
        }

        state_mask++;
    }
}

static inline void
gr_null_pdt_helper_body_unfold(s_gr_null_pdt_helper_t *null_pdt_helper,
    s_gr_body_list_t *body_list, s_gr_body_t *body)
{
    assert_exit(gr_null_pdt_helper_structure_legal_p(null_pdt_helper));
    assert_exit(gr_pdt_body_structure_legal_p(body));

    gr_null_pdt_helper_null_symbol_seek(null_pdt_helper, body);
    gr_null_pdt_helper_null_symbol_extend(null_pdt_helper, body_list, body);
}

static inline void
gr_null_pdt_helper_pdt_unfold(s_gr_null_pdt_helper_t *null_pdt_helper,
    s_gr_pdt_t *pdt)
{
    uint32 i, limit;
    s_gr_body_t *body;
    s_gr_body_list_t *body_list;

    assert_exit(gr_pdt_structure_legal_p(pdt));
    assert_exit(gr_null_pdt_helper_structure_legal_p(null_pdt_helper));

    i = 0;
    body_list = gr_pdt_body_list(pdt);
    limit = gr_body_list_body_limit(body_list);

    while (i < limit) {
        body = gr_body_list_body(body_list, i++);
        gr_null_pdt_helper_body_unfold(null_pdt_helper, body_list, body);
    }
}

static inline void
gr_null_pdt_helper_unfold(s_gr_null_pdt_helper_t *null_pdt_helper,
    s_gr_lang_t *gr_lang)
{
    uint32 i, limit;
    s_gr_pdt_t *pdt;

    assert_exit(gr_language_structure_legal_p(gr_lang));
    assert_exit(gr_null_pdt_helper_structure_legal_p(null_pdt_helper));

    i = 0;
    limit = gr_language_pdt_limit(gr_lang);

    while (i < limit) {
        pdt = gr_language_pdt(gr_lang, i++);
        gr_null_pdt_helper_pdt_unfold(null_pdt_helper, pdt);
    }

    GR_LANGUAGE_UNFOLD_PRINT(gr_lang);
}

static inline void
gr_pdt_null_body_eliminate(s_gr_pdt_t *pdt)
{
    uint32 i, k;
    s_gr_body_t *body;
    s_gr_symbol_t *symbol;
    s_gr_body_list_t *body_list;
    uint32 body_limit, symbol_limit;

    assert_exit(gr_pdt_structure_legal_p(pdt));

    i = 0;
    body_list = gr_pdt_body_list(pdt);
    body_limit = gr_body_list_body_limit(body_list);

    while (i < body_limit) {
        k = 0;
        body = gr_body_list_body(body_list, i);
        symbol_limit = gr_body_symbol_limit(body);

        while (k < symbol_limit) {
            symbol = gr_body_symbol(body, k);

            if (gr_symbol_null_p(symbol)) {
                gr_body_symbol_remove_and_destroy(body, k);
            }

            k++;
        }

        if (gr_body_empty_p(body)) {
            gr_body_list_body_remove_and_destroy(body_list, i);
        }

        i++;
    }
}

static inline void
gr_null_pdt_helper_null_pdt_eliminate(s_gr_null_pdt_helper_t *null_pdt_helper,
    s_gr_lang_t *gr_lang)
{
    s_gr_pdt_t *null_pdt;
    s_array_queue_t *null_pdt_queue;

    assert_exit(gr_language_structure_legal_p(gr_lang));
    assert_exit(gr_null_pdt_helper_structure_legal_p(null_pdt_helper));

    null_pdt_queue = gr_null_pdt_helper_null_pdt_queue(null_pdt_helper);

    while (!array_queue_empty_p(null_pdt_queue)) {
        null_pdt = array_queue_leave(null_pdt_queue);
        gr_pdt_null_body_eliminate(null_pdt);
    }

    GR_LANGUAGE_ELIMINATE_PRINT(gr_lang);
}

static inline void
gr_null_pdt_eliminate(s_gr_lang_t *gr_lang)
{
    s_gr_null_pdt_helper_t *null_pdt_helper;

    assert_exit(gr_language_structure_legal_p(gr_lang));

    null_pdt_helper = gr_null_pdt_helper_create();

    gr_null_pdt_helper_initial(null_pdt_helper, gr_lang);

    gr_null_pdt_helper_isolate(null_pdt_helper);

    gr_null_pdt_helper_unfold(null_pdt_helper, gr_lang);

    gr_null_pdt_helper_null_pdt_eliminate(null_pdt_helper, gr_lang);

    gr_null_pdt_helper_destroy(null_pdt_helper);
}

