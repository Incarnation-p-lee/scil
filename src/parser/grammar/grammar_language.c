static inline void
grammar_language_production_append(s_gr_lang_t *gr_lang, s_gr_pdt_t *gr_pdt)
{
    uint32 bytes_count;

    assert_exit(gr_lang && gr_pdt);

    if (gr_lang->index == gr_lang->size) {
        gr_lang->size = gr_lang->size * 2 + GR_PDT_LIST_MAX;
        bytes_count = sizeof(*gr_lang->pdt_list) * gr_lang->size;

        gr_lang->pdt_list = dp_realloc(gr_lang->pdt_list, bytes_count);
    }

    gr_lang->pdt_list[gr_lang->index++] = gr_pdt;
}

static inline s_gr_lang_t *
grammar_language_create(void)
{
    char **c;
    s_gr_pdt_t *gr_pdt;
    s_gr_lang_t *gr_lang;

    c = gr_language;

    gr_lang = dp_malloc(sizeof(*gr_lang));
    gr_lang->index = 0;
    gr_lang->size = GR_PDT_LIST_MAX;
    gr_lang->pdt_list = dp_malloc(sizeof(*gr_lang->pdt_list) * gr_lang->size);

    while (c < gr_language + ARRAY_SIZE_OF(gr_language)) {
        gr_pdt = grammar_production_create(*c);
        grammar_language_production_append(gr_lang, gr_pdt);

        c++;
    }

    return gr_lang;
}


static inline void
grammar_language_destroy(s_gr_lang_t *gr_lang)
{

    dp_free(gr_lang->pdt_list);
    dp_free(gr_lang);
}

