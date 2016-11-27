static inline uint32
token_language_c_nfa_match(s_nfa_t *nfa, char *buf)
{
    assert_exit(nfa && buf);

    return nfa_engine_pattern_match(nfa, buf);
}

uint32
token_language_c_operator_match(s_nfa_t *nfa, s_token_t *token_head, char *buf)
{
    s_token_t *token;
    uint32 match_size;

    if (!buf) {
        return TK_LANG_MATCH_INVALID;
    } else if (nfa_engine_structure_illegal_p(nfa)) {
        return TK_LANG_MATCH_INVALID;
    } else if (TOKEN_STRUCTURE_ILLEGAL_P(token_head)) {
        return TK_LANG_MATCH_INVALID;
    } else {
        TOKEN_LANGUAGE_C_OPTR_PRINT(buf);
        match_size = token_language_c_nfa_match(nfa, buf);

        if (match_size == NFA_SZ_INVALID) {
            scil_log_print_and_exit("Error in 'token_language_c_nfa_match'.\n");
        } else if (match_size) {
            token = token_language_c_optr_create(buf, match_size);
            token_list_insert_before(token_head, token);
        }
        return match_size;
    }
}

uint32
token_language_c_identifier_match(s_nfa_t *nfa, s_trie_tree_t *keyword_trie,
    s_token_t *token_head, char *buf)
{
    s_token_t *token;
    uint32 match_size;

    if (!buf) {
        return TK_LANG_MATCH_INVALID;
    } else if (nfa_engine_structure_illegal_p(nfa)) {
        return TK_LANG_MATCH_INVALID;
    } else if (TOKEN_STRUCTURE_ILLEGAL_P(token_head)) {
        return TK_LANG_MATCH_INVALID;
    } else if (!trie_tree_structure_legal_p(keyword_trie)) {
        return TK_LANG_MATCH_INVALID;
    } else {
        TOKEN_LANGUAGE_C_IDTR_PRINT(buf);
        match_size = token_language_c_nfa_match(nfa, buf);

        if (match_size == NFA_SZ_INVALID) {
            scil_log_print_and_exit("Error in 'token_language_c_nfa_match'.\n");
        } else if (match_size) {
            token = token_language_c_idtr_create(buf, match_size);
            token_language_c_keyword_seek(keyword_trie, token);
            token_list_insert_before(token_head, token);
        }

        return match_size;
    }
}

static inline void
token_language_c_keyword_seek(s_trie_tree_t *keyword_trie, s_token_t *token)
{
    s_token_language_c_idtr_t *tk_idtr;

    assert_exit(token_structure_legal_p(token));
    assert_exit(trie_tree_structure_legal_p(keyword_trie));

    tk_idtr = token->data;
    if (token_language_c_keyword_match_p(keyword_trie, tk_idtr->name)) {
        tk_idtr->is_keyword = true;
        token->type = TK_LEX_KWRD;
        TK_LANGUAGE_C_PRINT(token);
    }
}

uint32
token_language_c_constant_match(s_nfa_t *nfa, s_token_t *token_head, char *buf)
{
    s_token_t *token;
    uint32 match_size;

    if (!buf) {
        return TK_LANG_MATCH_INVALID;
    } else if (nfa_engine_structure_illegal_p(nfa)) {
        return TK_LANG_MATCH_INVALID;
    } else if (TOKEN_STRUCTURE_ILLEGAL_P(token_head)) {
        return TK_LANG_MATCH_INVALID;
    } else {
        TOKEN_LANGUAGE_C_CNST_PRINT(buf);
        match_size = token_language_c_nfa_match(nfa, buf);

        if (match_size == NFA_SZ_INVALID) {
            scil_log_print_and_exit("Error in 'token_language_c_nfa_match'.\n");
        } else if (match_size) {
            token = token_language_c_cnst_create(buf, match_size);
            token_list_insert_before(token_head, token);
        }

        return match_size;
    }
}

uint32
token_language_c_punctuation_match(s_nfa_t *nfa, s_token_t *token_head, char *buf)
{
    s_token_t *token;
    uint32 match_size;

    if (!buf) {
        return TK_LANG_MATCH_INVALID;
    } else if (nfa_engine_structure_illegal_p(nfa)) {
        return TK_LANG_MATCH_INVALID;
    } else if (TOKEN_STRUCTURE_ILLEGAL_P(token_head)) {
        return TK_LANG_MATCH_INVALID;
    } else {
        TOKEN_LANGUAGE_C_PCTT_PRINT(buf);
        match_size = token_language_c_nfa_match(nfa, buf);

        if (match_size == NFA_SZ_INVALID) {
            scil_log_print_and_exit("Error in 'token_language_c_nfa_match'.\n");
        } else if (match_size) {
            assert_exit(2 >= match_size);

            token = token_language_c_pctt_create(buf[0]);
            token_list_insert_before(token_head, token);
        }

        return match_size;
    }
}

static inline s_token_t *
token_language_c_optr_create(char *name, uint32 size)
{
    s_token_t *token;
    s_token_language_c_optr_t *optr;

    assert_exit(name);

    token = dp_malloc(sizeof(*token));
    token->type = TK_LEX_OPTR;
    token->data = dp_malloc(sizeof(s_token_language_c_optr_t));

    optr = token->data;
    optr->name = dp_malloc(sizeof(char) * (size + 1));
    dp_memcpy(optr->name, name, sizeof(*optr->name) * size);
    optr->name[size] = NULL_CHAR;

    doubly_linked_list_initial(&token->list);
    TK_LANGUAGE_C_PRINT(token);

    return token;
}

static inline s_token_t *
token_language_c_idtr_create(char *name, uint32 size)
{
    s_token_t *token;
    s_token_language_c_idtr_t *idtr;

    assert_exit(name && size);

    token = dp_malloc(sizeof(*token));
    token->type = TK_LEX_IDTR;
    token->data = idtr = dp_malloc(sizeof(s_token_language_c_idtr_t));

    idtr->is_keyword = false;
    idtr->name = dp_malloc(sizeof(*idtr->name) * (size + 1));
    dp_memcpy(idtr->name, name, sizeof(*idtr->name) * size);
    idtr->name[size] = NULL_CHAR;

    doubly_linked_list_initial(&token->list);
    TK_LANGUAGE_C_PRINT(token);

    return token;
}

static inline s_token_t *
token_language_c_cnst_create(char *buf, uint32 size)
{
    s_token_t *token;
    s_token_language_c_cnst_t *cnst;

    assert_exit(buf && size);

    token = dp_malloc(sizeof(*token));
    token->type = TK_LEX_CNST;
    token->data = cnst = dp_malloc(sizeof(s_token_language_c_cnst_t));

    cnst->name = dp_malloc(sizeof(*cnst->name) * (size + 1));
    dp_memcpy(cnst->name, buf, sizeof(*cnst->name) * (size + 1));
    cnst->name[size] = NULL_CHAR;

    doubly_linked_list_initial(&token->list);
    TK_LANGUAGE_C_PRINT(token);

    return token;
}

static inline s_token_t *
token_language_c_pctt_create(char c)
{
    s_token_t *token;
    s_token_language_c_pctt_t *pctt;

    assert_exit(token_language_c_pctt_char_p(c));

    token = dp_malloc(sizeof(*token));
    token->type = TK_LEX_PCTT;
    token->data = pctt = dp_malloc(sizeof(s_token_language_c_pctt_t));
    pctt->c = c;

    doubly_linked_list_initial(&token->list);
    TK_LANGUAGE_C_PRINT(token);

    return token;
}

s_trie_tree_t *
token_language_c_keyword_trie_create(void)
{
    s_trie_tree_t *keyword_trie;
    char **keyword, **keyword_limit;

    keyword = (void *)token_language_c_keywords;
    keyword_limit = keyword + ARRAY_SIZE_OF(token_language_c_keywords);
    keyword_trie = trie_tree_create();

    while (keyword < keyword_limit) {
        trie_tree_string_insert(keyword_trie, *keyword);
        keyword++;
    }

    return keyword_trie;
}

void
token_language_c_keyword_trie_destroy(s_trie_tree_t **keyword_trie)
{
    if (keyword_trie && trie_tree_structure_legal_p(*keyword_trie)) {
        trie_tree_destroy(keyword_trie);
    }
}

static inline bool
token_language_c_keyword_match_p(s_trie_tree_t *keyword_trie, char *name)
{
    assert_exit(name);
    assert_exit(trie_tree_structure_legal_p(keyword_trie));

    return trie_tree_string_matched_p(keyword_trie, name);
}

static inline void
token_language_c_node_destroy(s_token_t *token_node)
{
     s_token_language_c_idtr_t *token_idtr;
     s_token_language_c_optr_t *token_optr;
     s_token_language_c_cnst_t *token_cnst;

     assert_exit(token_node);

     switch (token_node->type) {
         case TK_LEX_HEAD:
             break;
         case TK_LEX_PCTT:
             dp_free(token_node->data);
             break;
         case TK_LEX_IDTR:
         case TK_LEX_KWRD:
             token_idtr = token_node->data;
             dp_free(token_idtr->name);
             dp_free(token_idtr);
             break;
         case TK_LEX_OPTR:
             token_optr = token_node->data;
             dp_free(token_optr->name);
             dp_free(token_optr);
             break;
         case TK_LEX_CNST:
             token_cnst = token_node->data;
             dp_free(token_cnst->name);
             dp_free(token_cnst);
             break;
         default:
             assert_exit(false);
             break;
     }

     dp_free(token_node);
}

void
token_language_c_destroy(s_token_t *token_list)
{
    s_token_t *token_node;
    s_token_t *token_next;

    if (TOKEN_STRUCTURE_LEGAL_P(token_list)) {
        token_node = token_list;
        do {
            token_next = token_list_node_next_i(token_node);
            token_language_c_node_destroy(token_node);
            token_node = token_next;
        } while (token_list != token_node);
    }
}

static inline bool
token_language_c_single_comment_p(char *buf)
{
    assert_exit(buf);

    if (TK_SLASH == buf[0] && TK_SLASH == buf[1]) {
        return true;
    } else {
        return false;
    }
}

static inline bool
token_language_c_multiple_comment_head_p(char *buf)
{
    assert_exit(buf);

    if (TK_SLASH == buf[0] && TK_STAR == buf[1]) {
        return true;
    } else {
        return false;
    }
}

static inline bool
token_language_c_multiple_comment_tail_p(char *buf)
{
    assert_exit(buf);

    if (TK_STAR == buf[0] && TK_SLASH == buf[1]) {
        return true;
    } else {
        return false;
    }
}

