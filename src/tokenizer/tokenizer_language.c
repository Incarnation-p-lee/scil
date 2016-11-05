static inline bool
tokenizer_language_type_legal_p(e_tokenizer_language_type_t language_type)
{
    switch (language_type) {
        case TKZ_LANG_C:
        case TKZ_LANG_CPP:
            return true;
        default:
            return false;
    }
}

static inline e_tokenizer_language_type_t
tokenizer_language_filename_to_type(char *filename)
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
        scil_log_print_and_exit("Unsupported language source file %s\n", filename);
        return TKZ_LANG_UNSUPPORTED;
    }
}

static inline s_tokenizer_language_t *
tokenizer_language_obtain(char *filename)
{
    uint32 index;
    e_tokenizer_language_type_t type;
    s_tokenizer_language_t *tkz_language;

    assert_exit(filename);

    type = tokenizer_language_filename_to_type(filename);
    index = (uint32)type;
    tkz_language = tkz_language_set[index];

    if (!tkz_language) {
        tkz_language = tokenizer_language_create(type);
        tkz_language_set[index] = tkz_language;
    }

    return tkz_language;
}

static inline s_tokenizer_language_t *
tokenizer_language_create(e_tokenizer_language_type_t type)
{
    s_tokenizer_language_t *tkz_language;

    assert_exit(tokenizer_language_type_legal_p(type));

    tkz_language = dp_malloc(sizeof(*tkz_language));

    tkz_language->type = type;
    tokenizer_language_init(tkz_language);

    return tkz_language;
}

static inline bool
tokenizer_language_type_p(e_tokenizer_language_type_t tkz_language_type)
{
    switch (tkz_language_type) {
        case TKZ_LANG_C:
        case TKZ_LANG_CPP:
            return true;
        default:
            return false;
    }
}

static inline bool
tokenizer_language_structure_legal_p(s_tokenizer_language_t *tkz_language)
{
    if (!tkz_language) {
        return false;
    } else if (!tkz_language->operator || !tkz_language->identifier
        || !tkz_language->constant || !tkz_language->punctuation) {
        return false;
    } else {
        return tokenizer_language_type_p(tkz_language->type);
    }
}

static inline void
tokenizer_language_cache_cleanup(void)
{
    s_tokenizer_language_t **tkz_iterator;

    tkz_iterator = tkz_language_set;

    while (tkz_iterator < tkz_language_set + ARRAY_SIZE_OF(tkz_language_set)) {
        if (*tkz_iterator) {
            tokenizer_language_destroy(*tkz_iterator);
        }
        tkz_iterator++;
    }
}

static inline void
tokenizer_language_destroy(s_tokenizer_language_t *tkz_language)
{
    assert_exit(tokenizer_language_structure_legal_p(tkz_language));

    switch (tkz_language->type) {
        case TKZ_LANG_C:
            tokenizer_language_c_destroy(tkz_language);
            break;
        default:
            assert_exit(false);
            break;
    }
}

static inline void
tokenizer_language_c_destroy(s_tokenizer_language_t *tkz_language)
{
    assert_exit(tokenizer_language_structure_legal_p(tkz_language));

    tokenizer_language_c_nfa_engine_destroy(tkz_language);
    tokenizer_language_c_keyword_trie_destroy(tkz_language);

    dp_free(tkz_language);
}

static inline void
tokenizer_language_c_keyword_trie_destroy(s_tokenizer_language_t *tkz_language)
{
    assert_exit(tkz_language);

    token_language_c_keyword_trie_destroy(&tkz_language->keyword_trie);
}

static inline void
tokenizer_language_c_nfa_engine_destroy(s_tokenizer_language_t *tkz_language)
{
    assert_exit(tkz_language);

    nfa_engine_destroy(tkz_language->identifier);
    nfa_engine_destroy(tkz_language->operator);
    nfa_engine_destroy(tkz_language->constant);
    nfa_engine_destroy(tkz_language->punctuation);
}

static inline void
tokenizer_language_init(s_tokenizer_language_t *tkz_language)
{
    assert_exit(tkz_language);

    switch (tkz_language->type) {
        case TKZ_LANG_C:
            tokenizer_language_c_init(tkz_language);
            break;
        default:
            assert_exit(false);
            break;
    }
}

static inline void
tokenizer_language_c_init(s_tokenizer_language_t *tkz_language)
{
    assert_exit(tkz_language);

    tokenizer_language_c_nfa_engine_init(tkz_language);
    tokenizer_language_c_keyword_trie_init(tkz_language);
}

static inline void
tokenizer_language_c_nfa_engine_init(s_tokenizer_language_t *tkz_language)
{
    assert_exit(tkz_language);

    tkz_language->identifier = nfa_engine_create(LANGUAGE_C_RE_IDTR);
    tkz_language->operator = nfa_engine_create(LANGUAGE_C_RE_OPTR);
    tkz_language->constant = nfa_engine_create(LANGUAGE_C_RE_CNST);
    tkz_language->punctuation = nfa_engine_create(LANGUAGE_C_RE_PCTT);
}

static inline void
tokenizer_language_c_keyword_trie_init(s_tokenizer_language_t *tkz_language)
{
    assert_exit(tkz_language);

    tkz_language->keyword_trie = token_language_c_keyword_trie_create();
}

static inline uint32
tokenizer_language_c_token_match(s_tokenizer_language_t *tkz_language,
    s_token_t *token_head, char *buf)
{
    uint32 match_size;
    s_nfa_t *nfa_engine;
    s_token_t *last_token;

    assert_exit(buf);
    assert_exit(token_structure_legal_p(token_head));
    assert_exit(tokenizer_language_structure_legal_p(tkz_language));

    nfa_engine = tkz_language->identifier;
    match_size = token_language_c_identifier_match(nfa_engine, token_head, buf);
    if (match_size != NFA_SZ_UNMATCH) {
        last_token = token_list_node_previous(token_head);
        token_language_c_keyword_seek(tkz_language->keyword_trie, last_token);
        return match_size;
    }

    nfa_engine = tkz_language->operator;
    match_size = token_language_c_operator_match(nfa_engine, token_head, buf);
    if (match_size != NFA_SZ_UNMATCH) {
        return match_size;
    }

    nfa_engine = tkz_language->constant;
    match_size = token_language_c_constant_match(nfa_engine, token_head, buf);
    if (match_size != NFA_SZ_UNMATCH) {
        return match_size;
    }

    nfa_engine = tkz_language->punctuation;
    match_size = token_language_c_punctuation_match(nfa_engine, token_head, buf);
    if (match_size != NFA_SZ_UNMATCH) {
        return match_size;
    }

    return 0;
}

