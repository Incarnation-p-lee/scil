static inline bool
tokenizer_lang_type_legal_p(e_token_lang_type_t lang_type)
{
    switch (lang_type) {
        case TK_LANG_C:
        case TK_LANG_CPP:
            return true;
        default:
            return false;
    }
}

static inline s_token_lang__t *
tokenizer_lang_create(char *filename)
{
    char *c;
    s_token_lang_t *lang;
    e_token_lang_type_t type;

    assert_exit(filename);

    c = dp_strrchr(filename, SPLIT_CHAR);
    c++;

    type = (e_token_lang_type_t)*c++;
    while (*c) {
        type = TK_OPTR_DUAL(type, (e_token_lang_type_t)*c++);
    }
    assert_exit(tokenizer_lang_type_legal_p(type));

    lang = dp_malloc(sizeof(*lang));
    lang->type = type;
    lang->sentinel = SENTINEL_CHAR;
    tokenizer_lang_nfa_init(lang);

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
            tokenizer_lang_c_init(lang);
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

