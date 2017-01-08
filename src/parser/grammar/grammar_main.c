void
grammar_analysis(void)
{
    s_gr_lang_t *gr_lang;

    gr_lang = grammar_language_create();

    grammar_language_destroy(gr_lang);
}

