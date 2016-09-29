static inline bool
tokenizer_language_type_legal_p(e_tokenizer_language_type_t language_type)
{
    switch (language_type) {
        case TK_LANG_C:
        case TK_LANG_CPP:
            return true;
        default:
            return false;
    }
}

static inline s_tokenizer_language_t *
tokenizer_language_create(char *filename)
{
    char *c;
    s_tokenizer_language_t *lang;
    e_tokenizer_language_type_t type;

    assert_exit(filename);

    c = dp_strrchr(filename, SPLIT_CHAR);
    c++;

    type = (e_tokenizer_language_type_t)*c++;
    while (*c) {
        type = TK_C_OPTR_DUAL(type, (e_tokenizer_language_type_t)*c++);
    }
    assert_exit(tokenizer_language_type_legal_p(type));

    lang = dp_malloc(sizeof(*lang));
    lang->type = type;
    tokenizer_language_init(lang);

    return lang;
}

static inline bool
tokenizer_language_type_p(e_tokenizer_language_type_t type)
{
    switch (type) {
        case TK_LANG_C:
        case TK_LANG_CPP:
            return true;
        default:
            return false;
    }
}

static inline bool
tokenizer_language_structure_legal_p(s_tokenizer_language_t *lang)
{
    if (!lang) {
        return false;
    } else if (!lang->operator || !lang->identifier || !lang->constant
        || !lang->punctuation) {
        return false;
    } else {
        return tokenizer_language_type_p(lang->type);
    }
}

static inline void
tokenizer_language_destroy(s_tokenizer_language_t *lang)
{
    assert_exit(tokenizer_language_structure_legal_p(lang));

    switch (lang->type) {
        case TK_LANG_C:
            tokenizer_language_c_destroy(lang);
        default:
            assert_exit(false);
            break;
    }
}

static inline void
tokenizer_language_c_destroy(s_tokenizer_language_t *lang)
{
    assert_exit(tokenizer_language_structure_legal_p(lang));

    tokenizer_language_c_nfa_engine_destroy(lang);
    tokenizer_language_c_keyword_trie_destroy(lang);

    dp_free(lang);
}

static inline void
tokenizer_language_c_keyword_trie_destroy(s_tokenizer_language_t *lang)
{
    assert_exit(lang);

    token_language_c_keyword_trie_destroy(lang->keyword_trie);
}

static inline void
tokenizer_language_c_nfa_engine_destroy(s_tokenizer_language_t *lang)
{
    assert_exit(lang);

    nfa_engine_destroy(lang->identifier);
    nfa_engine_destroy(lang->operator);
    nfa_engine_destroy(lang->constant);
    nfa_engine_destroy(lang->punctuation);
}

static inline void
tokenizer_language_init(s_tokenizer_language_t *lang)
{
    assert_exit(lang);

    switch (lang->type) {
        case TK_LANG_C:
            tokenizer_language_c_init(lang);
            break;
        default:
            assert_exit(false);
            break;
    }
}

static inline void
tokenizer_language_c_init(s_tokenizer_language_t *lang)
{
    assert_exit(lang);

    tokenizer_language_c_nfa_engine_init(lang);
    tokenizer_language_c_keyword_trie_init(lang);
}

static inline void
tokenizer_language_c_nfa_engine_init(s_tokenizer_language_t *lang)
{
    assert_exit(lang);

    lang->identifier = nfa_engine_create(LANGUAGE_C_RE_IDTR);
    lang->operator = nfa_engine_create(LANGUAGE_C_RE_OPTR);
    lang->constant = nfa_engine_create(LANGUAGE_C_RE_CNST);
    lang->punctuation = nfa_engine_create(LANGUAGE_C_RE_PCTT);
}

static inline void
tokenizer_language_c_keyword_trie_init(s_tokenizer_language_t *lang)
{
    assert_exit(lang);

    lang->keyword_trie = token_language_c_keyword_trie_create();
}

static inline uint32
tokenizer_language_operator_match(s_tokenizer_language_t *tkz_language,
    s_token_t *token_head, char *buf)
{
    assert_exit(buf);
    assert_exit(token_structure_legal_p(token_head));
    assert_exit(tokenizer_language_structure_legal_p(tkz_language));

    switch (tkz_language->type) {
        case TK_LANG_C:
            return token_language_c_operator_match(tkz_language->operator, token_head, buf);
        default:
            assert_exit(false);
            return 0;
    }
}

static inline uint32
tokenizer_lang_identifer_match(s_tokenizer_language_t *tkz_language,
    s_token_t *token_head, char *buf)
{
    uint32 n;
    s_token_t *token;

    assert_exit(buf);
    assert_exit(token_structure_legal_p(token_head));
    assert_exit(tokenizer_language_structure_legal_p(tkz_language));

    switch (tkz_language->type) {
        case TK_LANG_C:
            n = token_lang_c_identifier_match(tkz_language->identifier,
                token_head, buf);
            if (n) {
                token = token_list_previous_node(token_head);
                token_language_c_keyword_seek(lang->keyword_trie, token);
            }

            return n;
        default:
            assert_exit(false);
            return 0;
    }
}

static inline uint32
tokenizer_lang_constant_match(s_tokenizer_language_t *tkz_language,
    s_token_t *token_head, char *buf)
{
    assert_exit(buf);
    assert_exit(token_structure_legal_p(token_head));
    assert_exit(tokenizer_language_structure_legal_p(tkz_language));

    switch (tkz_language->type) {
        case TK_LANG_C:
            return token_lang_c_constant_match(tkz_language->constant,
                token_head, buf);
        default:
            assert_exit(false);
            return 0;
    }
}

static inline uint32
token_lang_punctuation_match(s_tokenizer_language_t *tkz_language,
    s_token_t *token_head, char *buf)
{
    assert_exit(buf);
    assert_exit(token_structure_legal_p(token_head));
    assert_exit(tokenizer_language_structure_legal_p(tkz_language));

    switch (tkz_language->type) {
        case TK_LANG_C:
            return token_lang_c_punctuation_match(tkz_language->operator,
                token_head, buf);
        default:
            assert_exit(false);
            return 0;
    }
}

