static inline bool
grammar_language_structure_legal_p(s_gr_lang_t *gr_lang)
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
grammar_production_structure_legal_p(s_gr_pdt_t *gr_pdt)
{
    if (!gr_pdt) {
        return false;
    } else if (!gr_pdt->name) {
        return false;
    } else if (grammar_non_terminal_structure_illegal_p(gr_pdt->head)) {
        return false;
    } else if (grammar_body_list_structure_illegal_p(gr_pdt->list)) {
        return false;
    } else {
        return true;
    }
}

static inline bool
grammar_non_terminal_structure_illegal_p(s_gr_non_tr_t *gr_non_tr)
{
    return !grammar_non_terminal_structure_legal_p(gr_non_tr);
}

static inline bool
grammar_non_terminal_structure_legal_p(s_gr_non_tr_t *gr_non_tr)
{
    if (!gr_non_tr) {
        return false;
    } else {
        return true;
    }
}

static inline bool
grammar_body_list_structure_illegal_p(s_gr_body_list_t *gr_body_list)
{
    return !grammar_body_list_structure_legal_p(gr_body_list);
}

static inline bool
grammar_body_list_structure_legal_p(s_gr_body_list_t *gr_body_list)
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
grammar_body_structure_legal_p(s_gr_body_t *gr_body)
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
grammar_symbol_structure_legal_p(s_gr_symbol_t *gr_symbol)
{
    if (!gr_symbol) {
        return false;
    } else {
        return true;
    }
}

