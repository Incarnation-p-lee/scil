static inline uint32
token_language_c_nfa_match(s_nfa_t *nfa, char *buf)
{
    assert_exit(nfa && buf);

    if (nfa_engine_pattern_match_p(nfa, buf)) {
        return dp_strlen(buf);
    } else {
        return NFA_SZ_UNMATCH;
    }
}

uint32
token_language_c_operator_match(s_nfa_t *nfa, s_token_t *token_head, char *buf)
{
    s_token_t *token;
    uint32 match_length;

    if (!buf) {
        return TK_LANG_UNMATCH;
    } else if (!nfa_engine_structure_legal_p(nfa)) {
        return TK_LANG_UNMATCH;
    } else if (!token_structure_legal_p(token_head)) {
        return TK_LANG_UNMATCH;
    } else {
        match_length = token_language_c_nfa_match(nfa, buf);

        if (match_length == NFA_SZ_INVALID) {
            scil_log_print_and_exit("Error occurs in 'token_language_c_nfa_match'.\n");
        } else if (match_length) {
            token = token_language_c_optr_create(buf);
            token_list_insert_before(token_head, token);
        }
        return match_length;
    }
}

uint32
token_language_c_identifier_match(s_nfa_t *nfa, s_token_t *token_head, char *buf)
{
    s_token_t *token;
    uint32 match_length;

    if (!buf) {
        return TK_LANG_UNMATCH;
    } else if (!nfa_engine_structure_legal_p(nfa)) {
        return TK_LANG_UNMATCH;
    } else if (!token_structure_legal_p(token_head)) {
        return TK_LANG_UNMATCH;
    } else {
        match_length = token_language_c_nfa_match(nfa, buf);

        if (match_length == NFA_SZ_INVALID) {
            scil_log_print_and_exit("Error occurs in 'token_language_c_nfa_match'.\n");
        } else if (match_length) {
            token = token_language_c_idtr_create(buf, match_length);
            token_list_insert_before(token_head, token);
        }

        return match_length;
    }
}

void
token_language_c_keyword_seek(s_trie_tree_t *keyword_trie, s_token_t *token)
{
    s_token_language_c_idtr_t *tk_idtr;

    if (!token_structure_legal_p(token)) {
        return;
    } else if (!trie_tree_structure_legal_p(keyword_trie)) {
        return;
    } else {
        tk_idtr = token->data;
        if (token_language_c_keyword_match_p(keyword_trie, tk_idtr->name)) {
            tk_idtr->is_keyword = true;
            token->type = TK_LEX_KWRD;
            TK_LANGUAGE_C_PRINT(token);
        }
    }
}

uint32
token_language_c_constant_match(s_nfa_t *nfa, s_token_t *token_head, char *buf)
{
    s_token_t *token;
    uint32 match_length;

    if (!buf) {
        return TK_LANG_UNMATCH;
    } else if (!nfa_engine_structure_legal_p(nfa)) {
        return TK_LANG_UNMATCH;
    } else if (!token_structure_legal_p(token_head)) {
        return TK_LANG_UNMATCH;
    } else {
        match_length = token_language_c_nfa_match(nfa, buf);

        if (match_length == NFA_SZ_INVALID) {
            scil_log_print_and_exit("Error occurs in 'token_language_c_nfa_match'.\n");
        } else if (match_length) {
            token = token_language_c_cnst_create(buf, match_length);
            token_list_insert_before(token_head, token);
        }

        return match_length;
    }
}

uint32
token_language_c_punctuation_match(s_nfa_t *nfa, s_token_t *token_head, char *buf)
{
    s_token_t *token;
    uint32 match_length;

    if (!buf) {
        return TK_LANG_UNMATCH;
    } else if (!nfa_engine_structure_legal_p(nfa)) {
        return TK_LANG_UNMATCH;
    } else if (!token_structure_legal_p(token_head)) {
        return TK_LANG_UNMATCH;
    } else {
        match_length = token_language_c_nfa_match(nfa, buf);

        if (match_length == NFA_SZ_INVALID) {
            scil_log_print_and_exit("Error occurs in 'token_language_c_nfa_match'.\n");
        } else if (match_length) {
            assert_exit(2 >= match_length);

            token = token_language_c_pctt_create(buf[0]);
            token_list_insert_before(token_head, token);
        }

        return match_length;
    }
}

static inline bool
token_language_c_pctt_type_p(e_token_language_c_pctt_type_t type)
{
    switch (type) {
        case TK_C_PCTT_COMMA:
        case TK_C_PCTT_S_BRKT_L:
        case TK_C_PCTT_S_BRKT_R:
        case TK_C_PCTT_M_BRKT_L:
        case TK_C_PCTT_M_BRKT_R:
        case TK_C_PCTT_L_BRKT_L:
        case TK_C_PCTT_L_BRKT_R:
        case TK_C_PCTT_SEMICOLON:
            return true;
        default:
            return false;
    }
}

static inline s_token_t *
token_language_c_optr_create(char *name)
{
    uint32 size;
    s_token_t *token;
    s_token_language_c_optr_t *optr;

    assert_exit(name);

    size = dp_strlen(name);

    token = dp_malloc(sizeof(*token));
    token->type = TK_LEX_OPTR;
    token->data = dp_malloc(sizeof(s_token_language_c_optr_t));

    optr = token->data;
    optr->name = dp_malloc(sizeof(char) * (size + 1));
    dp_strcpy(optr->name, name);

    doubly_linked_list_initial(&token->list);
    TK_LANGUAGE_C_PRINT(token);

    return token;
}

static inline s_token_t *
token_language_c_idtr_create(char *buf, uint32 size)
{
    s_token_t *token;
    s_token_language_c_idtr_t *idtr;

    assert_exit(buf && size);

    if (TK_SENTINEL != buf[size - 1]) {
        size++;
    }

    token = dp_malloc(sizeof(*token));
    token->type = TK_LEX_IDTR;
    token->data = idtr = dp_malloc(sizeof(s_token_language_c_idtr_t));

    idtr->is_keyword = false;
    idtr->name = dp_malloc(sizeof(*idtr->name) * size);
    dp_memcpy(idtr->name, buf, sizeof(*idtr->name) * size);
    idtr->name[size - 1] = NULL_CHAR;

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

    if (TK_SENTINEL != buf[size - 1]) {
        size++;
    }

    token = dp_malloc(sizeof(*token));
    token->type = TK_LEX_CNST;
    token->data = cnst = dp_malloc(sizeof(s_token_language_c_cnst_t));

    cnst->name = dp_malloc(sizeof(*cnst->name) * size);
    dp_memcpy(cnst->name, buf, sizeof(*cnst->name) * size);
    cnst->name[size - 1] = NULL_CHAR;

    doubly_linked_list_initial(&token->list);
    TK_LANGUAGE_C_PRINT(token);

    return token;
}

static inline s_token_t *
token_language_c_pctt_create(char c)
{
    s_token_t *token;
    s_token_language_c_pctt_t *pctt;
    e_token_language_c_pctt_type_t type;

    type = (e_token_language_c_pctt_type_t)c;

    assert_exit(token_language_c_pctt_type_p(type));

    token = dp_malloc(sizeof(*token));
    token->type = TK_LEX_PCTT;
    token->data = pctt = dp_malloc(sizeof(s_token_language_c_pctt_t));
    pctt->type = type;

    doubly_linked_list_initial(&token->list);
    TK_LANGUAGE_C_PRINT(token);

    return token;
}

static inline void
token_language_c_keyword_encode(char *encode_buf, char *keyword)
{
    assert_exit(keyword);

    // keyword to type encoded by first 4 char
    // if keyword has chars < 4, use TK_C_KYWD_HOLDER as placeholder
    dp_memset(encode_buf, TK_C_KYWD_HOLDER, TK_C_KYWD_ENCODE_SIZE);
    dp_memcpy(encode_buf, keyword, dp_strlen(keyword));
}

static inline e_token_language_c_kywd_type_t
token_language_c_keyword_to_type(char *keyword)
{
    char encode_buf[TK_C_KYWD_ENCODE_SIZE];

    assert_exit(keyword);
    assert_exit(token_language_c_keyword_legal_p(keyword));

    token_language_c_keyword_encode(encode_buf, keyword);

    return TK_4_CHAR_TO_U32(encode_buf);
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
         case TK_LEX_KWRD:
         case TK_LEX_PCTT:
             dp_free(token_node);
             break;
         case TK_LEX_IDTR:
             token_idtr = token_node->data;
             dp_free(token_idtr->name);
             dp_free(token_node);
         case TK_LEX_OPTR:
             token_optr = token_node->data;
             dp_free(token_optr->name);
             dp_free(token_node);
             break;
         case TK_LEX_CNST:
             token_cnst = token_node->data;
             dp_free(token_cnst->name);
             dp_free(token_node);
             break;
         default:
             assert_exit(false);
             break;
     }
}

void
token_language_c_destroy(s_token_t *token_list)
{
    s_token_t *token_node;
    s_token_t *token_next;

    if (!token_structure_legal_p(token_list)) {
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

static inline char
token_language_c_pctt_type_to_char(e_token_language_c_pctt_type_t type)
{
    assert_exit(token_language_c_pctt_type_p(type));

    return (char)type;
}

