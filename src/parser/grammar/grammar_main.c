static inline void
grammar_initial(s_gr_lang_t *gr_lang)
{
#if 0
    uint32 grammar_type;

    grammar_type = config_option_grammar_type();

    switch (grammar_type) {
        case GRAMMAR_TYPE_LL_1:
            break;
        default:
            log_print_and_exit("Unknown grammar type\n");
            break;
    }
#endif
    log_print("gr_lang %#x\n", gr_lang);
}

void
grammar_analysis(void)
{
    s_gr_lang_t *gr_lang;

    gr_lang = grammar_language_create();
    grammar_initial(gr_lang);

    grammar_language_destroy(gr_lang);
}

