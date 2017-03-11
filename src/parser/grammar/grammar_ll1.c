static inline void
gr_ll1_format(s_gr_lang_t *gr_lang)
{
    assert_exit(gr_language_structure_legal_p(gr_lang));

    gr_lang->gr_type = GR_TYPE_LL_1;

    gr_null_pdt_eliminate(gr_lang);
}

static inline void
gr_ll1_initial(s_gr_lang_t *gr_lang)
{
    assert_exit(gr_language_structure_legal_p(gr_lang));

    gr_ll1_format(gr_lang);
}

