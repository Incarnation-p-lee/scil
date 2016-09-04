s_token_t *
tokenizer_main(char *file)
{
    s_token_language_t *lang;

    lang = tokenizer_language_init(TK_LANG_C);
    tokenizer_language_c_destroy(lang);

    return NULL;
}

static inline s_token_language_t *
tokenizer_language_init(e_token_language_t lang_type)
{
    s_token_language_t *lang;

    lang = NULL;
    switch (lang_type) {
        case TK_LANG_C:
            lang = tokenizer_language_c_init();
            break;
        default:
            assert_exit(false);
            break;
    }

    return lang;
}

static inline s_token_language_t *
tokenizer_language_c_init(void)
{
    s_token_language_t *language;

    language = dp_malloc(sizeof(*language));

    language->identifier = nfa_engine_create(LANG_C_RE_IDTR);

    return language;
}

static void
tokenizer_language_c_destroy(s_token_language_t *lang)
{
    assert_exit(lang);

    nfa_engine_destroy(lang->identifier);
    dp_free(lang);
}

