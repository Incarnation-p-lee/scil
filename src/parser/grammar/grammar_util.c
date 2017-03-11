static inline uint32
gr_language_pdt_limit(s_gr_lang_t *gr_lang)
{
    assert_exit(gr_language_structure_legal_p(gr_lang));

    return gr_lang->index;
}

static inline s_gr_pdt_t *
gr_language_pdt(s_gr_lang_t *gr_lang, uint32 i)
{
    assert_exit(gr_language_structure_legal_p(gr_lang));
    assert_exit(i < gr_language_pdt_limit(gr_lang));

    return gr_lang->pdt_list[i];
}

static inline bool
gr_symbol_null_p(s_gr_symbol_t *symbol)
{
    assert_exit(gr_pdt_symbol_structure_legal_p(symbol));

    if (symbol->is_terminal && symbol->terminal.type == GR_TR_NULL) {
        return true;
    } else {
        return false;
    }
}

static inline bool
gr_body_null_p(s_gr_body_t *body)
{
    assert_exit(gr_pdt_body_structure_legal_p(body));

    if (gr_body_symbol_limit(body) == 1
        && gr_symbol_null_p(body->symbol_list[0])) {
        return true;
    } else {
        return false;
    }
}

static inline s_gr_body_list_t *
gr_pdt_body_list(s_gr_pdt_t *pdt)
{
    assert_exit(gr_pdt_structure_legal_p(pdt));

    return pdt->list;
}

static inline s_gr_non_tr_t *
gr_pdt_head(s_gr_pdt_t *pdt)
{
    assert_exit(gr_pdt_structure_legal_p(pdt));

    return pdt->head;
}

static inline e_gr_non_tr_type_t
gr_pdt_head_type(s_gr_pdt_t *pdt)
{
    s_gr_non_tr_t *gr_non_tr;

    assert_exit(gr_pdt_structure_legal_p(pdt));

    gr_non_tr = gr_pdt_head(pdt);

    return gr_non_tr->type;
}

static inline s_gr_symbol_t *
gr_body_symbol(s_gr_body_t *body, uint32 i)
{
    assert_exit(gr_pdt_body_structure_legal_p(body));
    assert_exit(i < gr_body_symbol_limit(body));

    return body->symbol_list[i];
}

static inline e_gr_non_tr_type_t
gr_symbol_non_terminal_type(s_gr_symbol_t *symbol)
{
    assert_exit(gr_pdt_symbol_structure_legal_p(symbol));
    assert_exit(gr_symbol_non_terminal_p(symbol));

    return symbol->non_terminal.type;
}

static inline bool
gr_symbol_non_terminal_p(s_gr_symbol_t *symbol)
{
    assert_exit(gr_pdt_symbol_structure_legal_p(symbol));

    return !gr_pdt_symbol_terminal_p(symbol);
}

static inline bool
gr_pdt_symbol_terminal_p(s_gr_symbol_t *symbol)
{
    assert_exit(gr_pdt_symbol_structure_legal_p(symbol));

    return symbol->is_terminal;
}

static inline uint32
gr_body_symbol_count(s_gr_body_t *body)
{
    assert_exit(gr_pdt_body_structure_legal_p(body));

    return body->index;
}

static inline uint32
gr_body_symbol_limit(s_gr_body_t *body)
{
    assert_exit(gr_pdt_body_structure_legal_p(body));

    return gr_body_symbol_count(body);
}

static inline s_gr_body_t *
gr_body_list_body(s_gr_body_list_t *body_list, uint32 i)
{
    assert_exit(gr_pdt_body_list_structure_legal_p(body_list));
    assert_exit(i < gr_body_list_body_limit(body_list));

    return body_list->body_list[i];
}

static inline uint32
gr_body_list_body_count(s_gr_body_list_t *body_list)
{
    assert_exit(gr_pdt_body_list_structure_legal_p(body_list));

    return body_list->index;
}

static inline uint32
gr_body_list_body_limit(s_gr_body_list_t *body_list)
{
    assert_exit(gr_pdt_body_list_structure_legal_p(body_list));

    return gr_body_list_body_count(body_list);
}

static inline bool
gr_pdt_null_body_contains_p(s_gr_pdt_t *pdt)
{
    uint32 i, limit;
    s_gr_body_t *body;
    s_gr_body_list_t *body_list;

    assert_exit(gr_pdt_structure_legal_p(pdt));

    body_list = pdt->list;

    i = 0;
    limit = gr_body_list_body_limit(body_list);

    while (i < limit) {
        body = body_list->body_list[i];

        if (gr_body_null_p(body)) {
            return true;
        }

        i++;
    }

    return false;
}

static inline uint32
gr_null_pdt_helper_null_pdt_count(s_gr_null_pdt_helper_t *null_pdt_helper)
{
    s_array_queue_t *null_pdt_queue;

    assert_exit(gr_null_pdt_helper_structure_legal_p(null_pdt_helper));

    null_pdt_queue = gr_null_pdt_helper_null_pdt_queue(null_pdt_helper);

    return array_queue_size(null_pdt_queue);
}

static inline s_array_queue_t *
gr_null_pdt_helper_null_pdt_queue(s_gr_null_pdt_helper_t *null_pdt_helper)
{
    assert_exit(gr_null_pdt_helper_structure_legal_p(null_pdt_helper));

    return null_pdt_helper->null_pdt_queue;
}

static inline s_array_queue_t *
gr_null_pdt_helper_pdt_queue(s_gr_null_pdt_helper_t *null_pdt_helper)
{
    assert_exit(gr_null_pdt_helper_structure_legal_p(null_pdt_helper));

    return null_pdt_helper->pdt_queue;
}

static inline s_bitmap_t *
gr_null_pdt_helper_null_pdt_bitmap(s_gr_null_pdt_helper_t *null_pdt_helper)
{
    assert_exit(gr_null_pdt_helper_structure_legal_p(null_pdt_helper));

    return null_pdt_helper->null_pdt_bitmap;
}

static inline void
gr_null_pdt_helper_null_pdt_add(s_gr_null_pdt_helper_t *null_pdt_helper,
    s_gr_pdt_t *pdt)
{
    s_bitmap_t *null_pdt_bitmap;
    s_array_queue_t *null_pdt_queue;
    native_wide_t symbol_type_value;

    assert_exit(gr_pdt_structure_legal_p(pdt));
    assert_exit(gr_null_pdt_helper_structure_legal_p(null_pdt_helper));

    null_pdt_queue = gr_null_pdt_helper_null_pdt_queue(null_pdt_helper);
    null_pdt_bitmap = gr_null_pdt_helper_null_pdt_bitmap(null_pdt_helper);
    symbol_type_value = (native_wide_t)gr_pdt_head_type(pdt);

    assert_exit(bitmap_bit_clear_p(null_pdt_bitmap, symbol_type_value));

    array_queue_enter(null_pdt_queue, pdt);
    bitmap_bit_set(null_pdt_bitmap, symbol_type_value);
}

static inline void
gr_null_pdt_helper_pdt_add(s_gr_null_pdt_helper_t *null_pdt_helper, s_gr_pdt_t *pdt)
{
    s_array_queue_t *pdt_queue;

    assert_exit(gr_pdt_structure_legal_p(pdt));
    assert_exit(gr_null_pdt_helper_structure_legal_p(null_pdt_helper));

    pdt_queue = gr_null_pdt_helper_pdt_queue(null_pdt_helper);
    array_queue_enter(pdt_queue, pdt);
}

static inline void
gr_body_symbol_append(s_gr_body_t *body, s_gr_symbol_t *symbol)
{
    assert_exit(gr_pdt_body_structure_legal_p(body));
    assert_exit(gr_pdt_symbol_structure_legal_p(symbol));

    body->symbol_list[body->index++] = symbol;
}

static inline void
gr_body_list_body_remove_and_destroy(s_gr_body_list_t *body_list, uint32 i)
{
    uint32 k;
    uint32 limit;
    s_gr_body_t *body;

    assert_exit(gr_pdt_body_list_structure_legal_p(body_list));
    assert_exit(i < gr_body_list_body_limit(body_list));

    body = gr_body_list_body(body_list, i);
    gr_body_destroy(body);

    k = i + 1;
    limit = gr_body_list_body_limit(body_list);

    while (k < limit) {
        body_list->body_list[i++] = body_list->body_list[k++];
    }

    body_list->index = limit - 1;
}

static inline void
gr_body_symbol_remove_and_destroy(s_gr_body_t *body, uint32 i)
{
    uint32 k;
    uint32 limit;
    s_gr_symbol_t *symbol;

    assert_exit(gr_pdt_body_structure_legal_p(body));
    assert_exit(i < gr_body_symbol_limit(body));

    symbol = gr_body_symbol(body, i);
    gr_symbol_destroy(symbol);

    k = i + 1;
    limit = gr_body_symbol_limit(body);

    while (k < limit) {
        body->symbol_list[i++] = body->symbol_list[k++];
    }

    body->index = limit - 1;
}

static inline bool
gr_body_empty_p(s_gr_body_t *body)
{
    assert_exit(gr_pdt_body_structure_legal_p(body));

    if (body->index == 0) {
        return true;
    } else {
        return false;
    }
}

static inline void
gr_body_list_body_append(s_gr_body_list_t *body_list,
    s_gr_body_t *body)
{
    assert_exit(gr_pdt_body_structure_legal_p(body));
    assert_exit(gr_pdt_body_list_structure_legal_p(body_list));

    body_list->body_list[body_list->index++] = body;
}

static inline uint32
gr_null_pdt_helper_index(s_gr_null_pdt_helper_t *null_pdt_helper, uint32 i)
{
    assert_exit(gr_null_pdt_helper_structure_legal_p(null_pdt_helper));
    assert_exit(i < gr_null_pdt_helper_index_list_size(null_pdt_helper));

    return null_pdt_helper->index_list[i];
}

static inline bool
gr_null_pdt_helper_state_on_p(s_gr_null_pdt_helper_t *null_pdt_helper, uint32 i)
{
    assert_exit(gr_null_pdt_helper_structure_legal_p(null_pdt_helper));
    assert_exit(i < gr_null_pdt_helper_state_list_size(null_pdt_helper));

    if (gr_null_pdt_helper_state(null_pdt_helper, i) == GR_SYMBOL_STATE_ON) {
        return true;
    } else {
        return false;
    }
}

static inline e_gr_symbol_state_t
gr_null_pdt_helper_state(s_gr_null_pdt_helper_t *null_pdt_helper, uint32 i)
{
    assert_exit(gr_null_pdt_helper_structure_legal_p(null_pdt_helper));
    assert_exit(i < gr_null_pdt_helper_state_list_size(null_pdt_helper));

    return null_pdt_helper->state_list[i];
}

static inline void
gr_null_pdt_helper_state_cleanup(s_gr_null_pdt_helper_t *null_pdt_helper)
{
    assert_exit(gr_null_pdt_helper_structure_legal_p(null_pdt_helper));

    null_pdt_helper->s = 0;
}

static inline void
gr_null_pdt_helper_index_cleanup(s_gr_null_pdt_helper_t *null_pdt_helper)
{
    assert_exit(gr_null_pdt_helper_structure_legal_p(null_pdt_helper));

    null_pdt_helper->i = 0;
}

static inline s_gr_symbol_t *
gr_body_symbol_copy(s_gr_body_t *body, uint32 i)
{
    s_gr_symbol_t *symbol;
    s_gr_symbol_t *copy_symbol;

    assert_exit(gr_pdt_body_structure_legal_p(body));
    assert_exit(i < gr_body_symbol_limit(body));

    symbol = gr_body_symbol(body, i);
    copy_symbol = dp_malloc(sizeof(*symbol));

    copy_symbol->is_terminal = symbol->is_terminal;
    copy_symbol->terminal = symbol->terminal;
    copy_symbol->non_terminal = symbol->non_terminal;

    return copy_symbol;
}

static inline void
gr_null_pdt_helper_index_list_append(s_gr_null_pdt_helper_t *null_pdt_helper,
    uint32 index)
{
    assert_exit(gr_null_pdt_helper_structure_legal_p(null_pdt_helper));

    null_pdt_helper->index_list[null_pdt_helper->i++] = index;
}

static inline uint32
gr_null_pdt_helper_index_list_size(s_gr_null_pdt_helper_t *null_pdt_helper)
{
    assert_exit(gr_null_pdt_helper_structure_legal_p(null_pdt_helper));

    return null_pdt_helper->i;
}

static inline void
gr_null_pdt_helper_state_list_append(s_gr_null_pdt_helper_t *null_pdt_helper,
    e_gr_symbol_state_t state)
{
    assert_exit(state != GR_SYMBOL_STATE_INVALID);
    assert_exit(gr_null_pdt_helper_structure_legal_p(null_pdt_helper));

    null_pdt_helper->state_list[null_pdt_helper->s++] = state;
}

static inline uint32
gr_null_pdt_helper_state_list_size(s_gr_null_pdt_helper_t *null_pdt_helper)
{
    assert_exit(gr_null_pdt_helper_structure_legal_p(null_pdt_helper));

    return null_pdt_helper->s;
}

