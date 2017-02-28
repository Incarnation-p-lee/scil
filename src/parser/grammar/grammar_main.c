void
gr_analysis(void)
{
    s_gr_lang_t *gr_lang;

    gr_lang = gr_language_create();

    gr_language_initial(gr_lang);

    gr_language_destroy(gr_lang);
}

