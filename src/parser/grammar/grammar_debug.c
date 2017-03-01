static inline bool
gr_pdt_null_symbol_set_structure_legal_p(s_gr_null_symbol_set_t *gr_null_set)
{
    if (!gr_null_set) {
        return false;
    } else if (array_queue_structure_illegal_p(gr_null_set->queue)) {
        return false;
    } else if (bitmap_structure_illegal_p(gr_null_set->bitmap)) {
        return false;
    } else {
        return true;
    }
}

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
            name = tr_type_name[gr_symbol->terminal.tr_type];
        } else {
            name = non_tr_type_name[gr_symbol->non_terminal.non_tr_type];
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

    name = non_tr_type_name[gr_pdt->head->non_tr_type];

    log_print("    +------------------------------------------------------\n");
    log_print("    | <%02d> pdt '%s'\n", idx, gr_pdt->name);
    log_print("    |     head = %s\n", name);

    gr_pdt_body_list_print(gr_pdt->list);

    log_print("    +------------------------------------------------------\n");
}

static inline void
gr_language_print(s_gr_lang_t *gr_lang)
{
    uint32 i;
    uint32 limit;
    s_gr_pdt_t *gr_pdt;

    assert_exit(gr_language_structure_legal_p(gr_lang));

    RETURN_IF_FALSE(config_grammar_verbose_p());

    log_print("[GRAMMAR] grammar language production list print:\n");

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
gr_null_pdt_set_print(s_gr_null_symbol_set_t *gr_null_set)
{
    uint32 i;
    s_gr_pdt_t *pdt;
    s_array_iterator_t *iterator;

    assert_exit(gr_pdt_null_symbol_set_structure_legal_p(gr_null_set));

    log_print("[GRAMMAR] grammar null inferred pdt list print:\n");

    i = 0;
    iterator = array_queue_iterator_obtain(gr_null_set->queue);
    iterator->fp_index_initial(gr_null_set->queue);

    while (iterator->fp_next_exist_p(gr_null_set->queue)) {
        pdt = iterator->fp_next_obtain(gr_null_set->queue);
        gr_pdt_print(pdt, i++);
    }
}

