static inline bool
gr_language_structure_legal_p(s_gr_lang_t *gr_lang)
{
    if (!gr_lang) {
        return false;
    } else if (!gr_lang->pdt_list) {
        return false;
    } else if (gr_lang->index > gr_lang->size) {
        return false;
    } else {
        return true;
    }
}

static inline bool
gr_pdt_structure_legal_p(s_gr_pdt_t *gr_pdt)
{
    if (!gr_pdt) {
        return false;
    } else if (!gr_pdt->name) {
        return false;
    } else if (gr_non_terminal_structure_illegal_p(gr_pdt->head)) {
        return false;
    } else if (gr_pdt_body_list_structure_illegal_p(gr_pdt->list)) {
        return false;
    } else {
        return true;
    }
}

static inline bool
gr_non_terminal_structure_illegal_p(s_gr_non_tr_t *gr_non_tr)
{
    return !gr_non_terminal_structure_legal_p(gr_non_tr);
}

static inline bool
gr_non_terminal_structure_legal_p(s_gr_non_tr_t *gr_non_tr)
{
    if (!gr_non_tr) {
        return false;
    } else {
        return true;
    }
}

static inline bool
gr_pdt_body_list_structure_illegal_p(s_gr_body_list_t *gr_body_list)
{
    return !gr_pdt_body_list_structure_legal_p(gr_body_list);
}

static inline bool
gr_pdt_body_list_structure_legal_p(s_gr_body_list_t *gr_body_list)
{
    if (!gr_body_list) {
        return false;
    } else if (gr_body_list->index > GR_BODY_LIST_MAX) {
        return false;
    } else {
        return true;
    }
}

static inline bool
gr_pdt_body_structure_legal_p(s_gr_body_t *gr_body)
{
    if (!gr_body) {
        return false;
    } else if (gr_body->index > GR_SYMBOL_LIST_MAX) {
        return false;
    } else {
        return true;
    }
}

static inline bool
gr_pdt_symbol_structure_legal_p(s_gr_symbol_t *gr_symbol)
{
    if (!gr_symbol) {
        return false;
    } else {
        return true;
    }
}

static inline bool
gr_null_pdt_helper_structure_legal_p(s_gr_null_pdt_helper_t *null_pdt_helper)
{
    if (!null_pdt_helper) {
        return false;
    } else if (!null_pdt_helper->pdt_queue) {
        return false;
    } else if (!null_pdt_helper->null_pdt_queue) {
        return false;
    } else if (!null_pdt_helper->null_pdt_bitmap) {
        return false;
    } else if (null_pdt_helper->i > GR_SYMBOL_LIST_MAX) {
        return false;
    } else if (null_pdt_helper->s > GR_SYMBOL_LIST_MAX) {
        return false;
    } else {
        return true;
    }
}

static inline void
gr_pdt_body_print(s_gr_body_t *gr_body)
{
    uint32 i;
    uint32 limit;
    const char *name;
    s_gr_symbol_t *gr_symbol;

    assert_exit(gr_pdt_body_structure_legal_p(gr_body));

    log_print("    |          >");

    i = 0;
    limit = gr_body->index;

    while (i < limit) {
        gr_symbol = gr_body->symbol_list[i];

        if (gr_symbol->is_terminal) {
            name = tr_type_name[gr_symbol->terminal.type];
        } else {
            name = non_tr_type_name[gr_symbol->non_terminal.type];
        }

        log_print(" %s", name);

        i++;
    }

    log_print("\n");
}

static inline void
gr_pdt_body_list_print(s_gr_body_list_t *gr_body_list)
{
    uint32 i;
    uint32 limit;

    assert_exit(gr_pdt_body_list_structure_legal_p(gr_body_list));

    log_print("    |     body_list\n");

    i = 0;
    limit = gr_body_list->index;

    while (i < limit) {
        gr_pdt_body_print(gr_body_list->body_list[i++]);
    }
}

static inline void
gr_pdt_print(s_gr_pdt_t *gr_pdt, uint32 idx)
{
    const char *name;

    assert_exit(gr_pdt_structure_legal_p(gr_pdt));

    name = non_tr_type_name[gr_pdt->head->type];

    log_print("    +------------------------------------------------------\n");
    log_print("    | <%02d> pdt '%s'\n", idx, gr_pdt->name);
    log_print("    |     head = %s\n", name);

    gr_pdt_body_list_print(gr_pdt->list);

    log_print("    +------------------------------------------------------\n");
}

static inline void
gr_language_print_i(s_gr_lang_t *gr_lang)
{
    uint32 i;
    uint32 limit;
    s_gr_pdt_t *gr_pdt;

    assert_exit(gr_language_structure_legal_p(gr_lang));

    i = 0;
    limit = gr_lang->index;

    while (i < limit) {
        gr_pdt = gr_lang->pdt_list[i];
        gr_pdt_print(gr_pdt, i);

        i++;
    }

    log_print("\n");
}

static inline void
gr_language_print(s_gr_lang_t *gr_lang)
{
    assert_exit(gr_language_structure_legal_p(gr_lang));

    RETURN_IF_FALSE(config_grammar_verbose_p());

    log_print("[GRAMMAR] grammar language print:\n");

    gr_language_print_i(gr_lang);
}

static inline void
gr_language_after_unfold_print(s_gr_lang_t *gr_lang)
{
    assert_exit(gr_language_structure_legal_p(gr_lang));

    RETURN_IF_FALSE(config_grammar_verbose_p());

    log_print("[GRAMMAR] grammar language after unfold print:\n");

    gr_language_print_i(gr_lang);
}

static inline void
gr_language_eliminate_print(s_gr_lang_t *gr_lang)
{
    assert_exit(gr_language_structure_legal_p(gr_lang));

    RETURN_IF_FALSE(config_grammar_verbose_p());

    log_print("[GRAMMAR] grammar language after null eliminate print:\n");

    gr_language_print_i(gr_lang);
}

static inline void
gr_null_pdt_helper_print(s_gr_null_pdt_helper_t *null_pdt_helper)
{
    uint32 i;
    s_gr_pdt_t *pdt;
    s_array_queue_t *queue;
    s_array_iterator_t *iterator;

    assert_exit(gr_null_pdt_helper_structure_legal_p(null_pdt_helper));

    log_print("[GRAMMAR] grammar null pdt helper print:\n");

    i = 0;
    queue = gr_null_pdt_helper_null_pdt_queue(null_pdt_helper);
    log_print("    | null inferred pdt:\n");

    iterator = array_queue_iterator_obtain(queue);
    iterator->fp_index_initial(queue);

    while (iterator->fp_next_exist_p(queue)) {
        pdt = iterator->fp_next_obtain(queue);
        gr_pdt_print(pdt, i++);
    }


    i = 0;
    queue = gr_null_pdt_helper_null_pdt_queue(null_pdt_helper);
    log_print("    | non-null inferred pdt:\n");

    iterator = array_queue_iterator_obtain(queue);
    iterator->fp_index_initial(queue);

    while (iterator->fp_next_exist_p(queue)) {
        pdt = iterator->fp_next_obtain(queue);
        gr_pdt_print(pdt, i++);
    }
}

