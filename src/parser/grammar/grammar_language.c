static inline void
gr_language_pdt_append(s_gr_lang_t *gr_lang, s_gr_pdt_t *gr_pdt)
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
gr_language_create(void)
{
    char **c;
    s_gr_pdt_t *gr_pdt;
    s_gr_lang_t *gr_lang;

    c = gr_language;

    gr_lang = dp_malloc(sizeof(*gr_lang));

    gr_lang->index = 0;
    gr_lang->size = GR_PDT_LIST_MAX;
    gr_lang->gr_type = GR_TYPE_UNKNOWN;
    gr_lang->data = NULL;

    gr_lang->pdt_list = dp_malloc(sizeof(*gr_lang->pdt_list) * gr_lang->size);

    while (c < gr_language + ARRAY_SIZE_OF(gr_language)) {
        gr_pdt = gr_pdt_create(*c);
        gr_language_pdt_append(gr_lang, gr_pdt);

        c++;
    }

    GR_LANGUAGE_PRINT(gr_lang);
    return gr_lang;
}

static inline void
gr_language_destroy(s_gr_lang_t *gr_lang)
{
    uint32 i;
    uint32 limit;

    assert_exit(gr_language_structure_legal_p(gr_lang));

    i = 0;
    limit = gr_lang->index;

    while (i < limit) {
        gr_pdt_destroy(gr_lang->pdt_list[i]);
        i++;
    }

    dp_free(gr_lang->pdt_list);
    dp_free(gr_lang);
}

static inline void
gr_language_initial(s_gr_lang_t *gr_lang)
{
    uint32 gr_type;

    assert_exit(gr_language_structure_legal_p(gr_lang));

    gr_type = config_grammar_type();

    switch (gr_type) {
        case GR_TYPE_LL_1:
            gr_ll1_initial(gr_lang);
            break;
        default:
            log_print_and_exit("Unknown grammar type.\n");
            break;
    }
}

