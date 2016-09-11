static inline s_token_lang_t *
tokenizer_lang_create(e_token_lang_t lang_type)
{
    s_token_lang_t *lang;

    lang = dp_malloc(sizeof(*lang));
    lang->type = lang_type;
    tokenizer_lang_nfa_init(lang_type);

    return lang;
}

static inline bool
tokenizer_lang_structure_legal_p(s_token_lang_t *lang)
{
    if (!lang) {
        return false;
    } else if (TK_LANG_FISRT > lang->type || TK_LANG_LAST < lang->type) {
        return false;
    } else if (!lang->operator || !lang->identifier || !lang->constant
        || !lang->punctuation) {
        return false;
    } else {
        return true;
    }
}

static inline void
tokenizer_lang_destroy(s_token_lang_t *lang)
{
    assert_exit(lang);

    nfa_engine_destroy(lang->identifier);
    nfa_engine_destroy(lang->operator);
    nfa_engine_destroy(lang->constant);
    nfa_engine_destroy(lang->punctuation);
    dp_free(lang);
}

static inline void
tokenizer_lang_nfa_init(s_token_lang_t *lang)
{
    assert_exit(lang);

    switch (lang->type) {
        case TK_LANG_C:
            lang = tokenizer_lang_c_init(lang);
            break;
        default:
            assert_exit(false);
            break;
    }
}

static inline void
tokenizer_lang_c_init(s_token_lang_t *lang)
{
    assert_exit(lang);

    lang->identifier = nfa_engine_create(LANG_C_RE_IDTR);
    lang->operator = nfa_engine_create(LANG_C_RE_OPTR);
    lang->constant = nfa_engine_create(LANG_C_RE_CNST);
    lang->punctuation = nfa_engine_create(LANG_C_RE_PCTT);
}

