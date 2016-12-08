static inline e_tkz_lang_type_t
tkz_lang_filename_to_type(char *filename)
{
    char *c;

    assert_exit(filename);

    c = dp_strrchr(filename, TK_FILE_SPLIT);
    c++;

    if (dp_strcmp(c, "c") == 0) {
        return TKZ_LANG_C;
    } else if (dp_strcmp(c, "cpp") == 0) {
        return TKZ_LANG_CPP;
    } else {
        log_print_and_exit("Unsupported lang source file %s\n", filename);
        return TKZ_LANG_UNSUPPORTED;
    }
}

static inline s_tkz_lang_t *
tkz_lang_obtain(char *filename)
{
    uint32 index;
    e_tkz_lang_type_t type;
    s_tkz_lang_t *tkz_lang;

    assert_exit(filename);

    type = tkz_lang_filename_to_type(filename);
    index = (uint32)type;
    tkz_lang = tkz_lang_set[index];

    if (!tkz_lang) {
        tkz_lang = tkz_lang_create(type);
        tkz_lang_set[index] = tkz_lang;
    }

    return tkz_lang;
}

static inline s_tkz_lang_t *
tkz_lang_create(e_tkz_lang_type_t type)
{
    s_tkz_lang_t *tkz_lang;

    assert_exit(tkz_lang_type_legal_p(type));

    tkz_lang = dp_malloc(sizeof(*tkz_lang));

    tkz_lang->type = type;
    tkz_lang_init(tkz_lang);

    return tkz_lang;
}

static inline void
tkz_lang_cache_cleanup(void)
{
    s_tkz_lang_t **tkz_iterator;

    tkz_iterator = tkz_lang_set;

    while (tkz_iterator < tkz_lang_set + ARRAY_SIZE_OF(tkz_lang_set)) {
        if (*tkz_iterator) {
            tkz_lang_destroy(*tkz_iterator);
        }
        tkz_iterator++;
    }
}

static inline void
tkz_lang_destroy(s_tkz_lang_t *tkz_lang)
{
    assert_exit(tkz_lang_structure_legal_p(tkz_lang));

    switch (tkz_lang->type) {
        case TKZ_LANG_C:
            tkz_lang_c_destroy(tkz_lang);
            break;
        default:
            assert_exit(false);
            break;
    }
}

static inline void
tkz_lang_c_destroy(s_tkz_lang_t *tkz_lang)
{
    assert_exit(tkz_lang_structure_legal_p(tkz_lang));

    tkz_lang_c_nfa_engine_destroy(tkz_lang);
    tkz_lang_c_keyword_trie_destroy(tkz_lang);

    dp_free(tkz_lang);
}

static inline void
tkz_lang_c_keyword_trie_destroy(s_tkz_lang_t *tkz_lang)
{
    assert_exit(tkz_lang);

    tk_lang_c_keyword_trie_destroy(&tkz_lang->keyword_trie);
}

static inline void
tkz_lang_c_nfa_engine_destroy(s_tkz_lang_t *tkz_lang)
{
    assert_exit(tkz_lang);

    nfa_engine_destroy(tkz_lang->identifier);
    nfa_engine_destroy(tkz_lang->operator);
    nfa_engine_destroy(tkz_lang->constant);
    nfa_engine_destroy(tkz_lang->punctuation);
}

static inline void
tkz_lang_init(s_tkz_lang_t *tkz_lang)
{
    assert_exit(tkz_lang);

    switch (tkz_lang->type) {
        case TKZ_LANG_C:
            tkz_lang_c_init(tkz_lang);
            break;
        default:
            assert_exit(false);
            break;
    }
}

static inline void
tkz_lang_c_init(s_tkz_lang_t *tkz_lang)
{
    assert_exit(tkz_lang);

    tkz_lang_c_nfa_engine_init(tkz_lang);
    tkz_lang_c_keyword_trie_init(tkz_lang);
}

static inline void
tkz_lang_c_nfa_engine_init(s_tkz_lang_t *tkz_lang)
{
    assert_exit(tkz_lang);

    tkz_lang->identifier = nfa_engine_create(LANG_C_RE_IDTR);
    tkz_lang->operator = nfa_engine_create(LANG_C_RE_OPTR);
    tkz_lang->constant = nfa_engine_create(LANG_C_RE_CNST);
    tkz_lang->punctuation = nfa_engine_create(LANG_C_RE_PCTT);
}

static inline void
tkz_lang_c_keyword_trie_init(s_tkz_lang_t *tkz_lang)
{
    assert_exit(tkz_lang);

    tkz_lang->keyword_trie = tk_lang_c_keyword_trie_create();
}

static inline uint32
tkz_lang_c_tk_match(s_tkz_lang_t *tkz_lang,
    s_tk_t *tk_head, char *buf)
{
    uint32 match_size;
    s_nfa_t *nfa_engine;

    assert_exit(buf);
    assert_exit(tk_structure_legal_p(tk_head));
    assert_exit(tkz_lang_structure_legal_p(tkz_lang));

    nfa_engine = tkz_lang->identifier;
    match_size = tk_lang_c_idtr_match(nfa_engine,
        tkz_lang->keyword_trie, tk_head, buf);
    RETURN_FIR_IF_NE(match_size, NFA_SZ_UNMATCH);

    nfa_engine = tkz_lang->operator;
    match_size = tk_lang_c_optr_match(nfa_engine, tk_head, buf);
    RETURN_FIR_IF_NE(match_size, NFA_SZ_UNMATCH);

    nfa_engine = tkz_lang->constant;
    match_size = tk_lang_c_cnst_match(nfa_engine, tk_head, buf);
    RETURN_FIR_IF_NE(match_size, NFA_SZ_UNMATCH);

    nfa_engine = tkz_lang->punctuation;
    match_size = tk_lang_c_pctt_match(nfa_engine, tk_head, buf);
    RETURN_FIR_IF_NE(match_size, NFA_SZ_UNMATCH);

    return NFA_SZ_UNMATCH;
}

