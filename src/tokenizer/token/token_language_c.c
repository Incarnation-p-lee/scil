static inline uint32
tk_lang_c_nfa_match(s_nfa_t *nfa, char *buf)
{
    assert_exit(nfa && buf);

    return nfa_engine_pattern_match(nfa, buf);
}

uint32
tk_lang_c_optr_match(s_nfa_t *nfa, s_tk_t *tk_head, char *buf)
{
    s_tk_t *token;
    uint32 match_size;

    if (!buf) {
        return TK_LANG_MATCH_INVALID;
    } else if (nfa_engine_structure_illegal_p(nfa)) {
        return TK_LANG_MATCH_INVALID;
    } else if (TK_STRUCTURE_ILLEGAL_P(tk_head)) {
        return TK_LANG_MATCH_INVALID;
    } else {
        TK_LANG_C_OPTR_PRINT(buf);
        match_size = tk_lang_c_nfa_match(nfa, buf);

        if (match_size == NFA_SZ_INVALID) {
            scil_log_print_and_exit("Error in 'tk_lang_c_nfa_match'.\n");
        } else if (match_size) {
            token = tk_lang_c_optr_create(buf, match_size);
            tk_list_insert_before(tk_head, token);
        }
        return match_size;
    }
}

uint32
tk_lang_c_idtr_match(s_nfa_t *nfa, s_trie_tree_t *keyword_trie,
    s_tk_t *tk_head, char *buf)
{
    s_tk_t *token;
    uint32 match_size;

    if (!buf) {
        return TK_LANG_MATCH_INVALID;
    } else if (nfa_engine_structure_illegal_p(nfa)) {
        return TK_LANG_MATCH_INVALID;
    } else if (TK_STRUCTURE_ILLEGAL_P(tk_head)) {
        return TK_LANG_MATCH_INVALID;
    } else if (!trie_tree_structure_legal_p(keyword_trie)) {
        return TK_LANG_MATCH_INVALID;
    } else {
        TK_LANG_C_IDTR_PRINT(buf);
        match_size = tk_lang_c_nfa_match(nfa, buf);

        if (match_size == NFA_SZ_INVALID) {
            scil_log_print_and_exit("Error in 'tk_lang_c_nfa_match'.\n");
        } else if (match_size) {
            token = tk_lang_c_idtr_create(buf, match_size);
            tk_lang_c_keyword_seek(keyword_trie, token);
            tk_list_insert_before(tk_head, token);
        }

        return match_size;
    }
}

static inline void
tk_lang_c_keyword_seek(s_trie_tree_t *keyword_trie, s_tk_t *token)
{
    s_tk_lang_c_idtr_t *tk_idtr;

    assert_exit(tk_structure_legal_p(token));
    assert_exit(trie_tree_structure_legal_p(keyword_trie));

    tk_idtr = token->data;
    if (tk_lang_c_keyword_match_p(keyword_trie, tk_idtr->name)) {
        tk_idtr->is_keyword = true;
        token->type = TK_LEX_KWRD;
        TK_LANG_C_PRINT(token);
    }
}

uint32
tk_lang_c_cnst_match(s_nfa_t *nfa, s_tk_t *tk_head, char *buf)
{
    s_tk_t *token;
    uint32 match_size;

    if (!buf) {
        return TK_LANG_MATCH_INVALID;
    } else if (nfa_engine_structure_illegal_p(nfa)) {
        return TK_LANG_MATCH_INVALID;
    } else if (TK_STRUCTURE_ILLEGAL_P(tk_head)) {
        return TK_LANG_MATCH_INVALID;
    } else {
        TK_LANG_C_CNST_PRINT(buf);
        match_size = tk_lang_c_nfa_match(nfa, buf);

        if (match_size == NFA_SZ_INVALID) {
            scil_log_print_and_exit("Error in 'tk_lang_c_nfa_match'.\n");
        } else if (match_size) {
            token = tk_lang_c_cnst_create(buf, match_size);
            tk_list_insert_before(tk_head, token);
        }

        return match_size;
    }
}

uint32
tk_lang_c_pctt_match(s_nfa_t *nfa, s_tk_t *tk_head, char *buf)
{
    s_tk_t *token;
    uint32 match_size;

    if (!buf) {
        return TK_LANG_MATCH_INVALID;
    } else if (nfa_engine_structure_illegal_p(nfa)) {
        return TK_LANG_MATCH_INVALID;
    } else if (TK_STRUCTURE_ILLEGAL_P(tk_head)) {
        return TK_LANG_MATCH_INVALID;
    } else {
        TK_LANG_C_PCTT_PRINT(buf);
        match_size = tk_lang_c_nfa_match(nfa, buf);

        if (match_size == NFA_SZ_INVALID) {
            scil_log_print_and_exit("Error in 'tk_lang_c_nfa_match'.\n");
        } else if (match_size) {
            assert_exit(2 >= match_size);

            token = tk_lang_c_pctt_create(buf[0]);
            tk_list_insert_before(tk_head, token);
        }

        return match_size;
    }
}

static inline s_tk_t *
tk_lang_c_optr_create(char *name, uint32 size)
{
    s_tk_t *token;
    s_tk_lang_c_optr_t *optr;

    assert_exit(name);

    token = dp_malloc(sizeof(*token));
    token->type = TK_LEX_OPTR;
    token->data = dp_malloc(sizeof(s_tk_lang_c_optr_t));

    optr = token->data;
    optr->name = dp_malloc(sizeof(char) * (size + 1));
    dp_memcpy(optr->name, name, sizeof(*optr->name) * size);
    optr->name[size] = NULL_CHAR;

    doubly_linked_list_initial(&token->list);
    TK_LANG_C_PRINT(token);

    return token;
}

static inline s_tk_t *
tk_lang_c_idtr_create(char *name, uint32 size)
{
    s_tk_t *token;
    s_tk_lang_c_idtr_t *idtr;

    assert_exit(name && size);

    token = dp_malloc(sizeof(*token));
    token->type = TK_LEX_IDTR;
    token->data = idtr = dp_malloc(sizeof(s_tk_lang_c_idtr_t));

    idtr->is_keyword = false;
    idtr->name = dp_malloc(sizeof(*idtr->name) * (size + 1));
    dp_memcpy(idtr->name, name, sizeof(*idtr->name) * size);
    idtr->name[size] = NULL_CHAR;

    doubly_linked_list_initial(&token->list);
    TK_LANG_C_PRINT(token);

    return token;
}

static inline s_tk_t *
tk_lang_c_cnst_create(char *buf, uint32 size)
{
    s_tk_t *token;
    s_tk_lang_c_cnst_t *cnst;

    assert_exit(buf && size);

    token = dp_malloc(sizeof(*token));
    token->type = TK_LEX_CNST;
    token->data = cnst = dp_malloc(sizeof(s_tk_lang_c_cnst_t));

    cnst->name = dp_malloc(sizeof(*cnst->name) * (size + 1));
    dp_memcpy(cnst->name, buf, sizeof(*cnst->name) * (size + 1));
    cnst->name[size] = NULL_CHAR;

    doubly_linked_list_initial(&token->list);
    TK_LANG_C_PRINT(token);

    return token;
}

static inline s_tk_t *
tk_lang_c_pctt_create(char c)
{
    s_tk_t *token;
    s_tk_lang_c_pctt_t *pctt;

    assert_exit(tk_lang_c_pctt_char_p(c));

    token = dp_malloc(sizeof(*token));
    token->type = TK_LEX_PCTT;
    token->data = pctt = dp_malloc(sizeof(s_tk_lang_c_pctt_t));
    pctt->c = c;

    doubly_linked_list_initial(&token->list);
    TK_LANG_C_PRINT(token);

    return token;
}

s_trie_tree_t *
tk_lang_c_keyword_trie_create(void)
{
    s_trie_tree_t *keyword_trie;
    char **keyword, **keyword_limit;

    keyword = (void *)tk_lang_c_keywords;
    keyword_limit = keyword + ARRAY_SIZE_OF(tk_lang_c_keywords);
    keyword_trie = trie_tree_create();

    while (keyword < keyword_limit) {
        trie_tree_string_insert(keyword_trie, *keyword);
        keyword++;
    }

    return keyword_trie;
}

void
tk_lang_c_keyword_trie_destroy(s_trie_tree_t **keyword_trie)
{
    if (keyword_trie && trie_tree_structure_legal_p(*keyword_trie)) {
        trie_tree_destroy(keyword_trie);
    }
}

static inline bool
tk_lang_c_keyword_match_p(s_trie_tree_t *keyword_trie, char *name)
{
    assert_exit(name);
    assert_exit(trie_tree_structure_legal_p(keyword_trie));

    return trie_tree_string_matched_p(keyword_trie, name);
}

static inline void
tk_lang_c_node_destroy(s_tk_t *tk_node)
{
     s_tk_lang_c_idtr_t *tk_idtr;
     s_tk_lang_c_optr_t *tk_optr;
     s_tk_lang_c_cnst_t *tk_cnst;

     assert_exit(tk_node);

     switch (tk_node->type) {
         case TK_LEX_HEAD:
             break;
         case TK_LEX_PCTT:
             dp_free(tk_node->data);
             break;
         case TK_LEX_IDTR:
         case TK_LEX_KWRD:
             tk_idtr = tk_node->data;
             dp_free(tk_idtr->name);
             dp_free(tk_idtr);
             break;
         case TK_LEX_OPTR:
             tk_optr = tk_node->data;
             dp_free(tk_optr->name);
             dp_free(tk_optr);
             break;
         case TK_LEX_CNST:
             tk_cnst = tk_node->data;
             dp_free(tk_cnst->name);
             dp_free(tk_cnst);
             break;
         default:
             assert_exit(false);
             break;
     }

     dp_free(tk_node);
}

void
tk_lang_c_destroy(s_tk_t *tk_list)
{
    s_tk_t *tk_node;
    s_tk_t *tk_next;

    if (TK_STRUCTURE_LEGAL_P(tk_list)) {
        tk_node = tk_list;
        do {
            tk_next = tk_list_node_next_i(tk_node);
            tk_lang_c_node_destroy(tk_node);
            tk_node = tk_next;
        } while (tk_list != tk_node);
    }
}

static inline char
tk_lang_c_single_comment_end(void)
{
    return TK_NEWLINE;
}

static inline bool
tk_lang_c_single_comment_p(char *buf)
{
    assert_exit(buf);

    if (TK_SLASH == buf[0] && TK_SLASH == buf[1]) {
        return true;
    } else {
        return false;
    }
}

static inline bool
tk_lang_c_multiple_comment_head_p(char *buf)
{
    assert_exit(buf);

    if (TK_SLASH == buf[0] && TK_STAR == buf[1]) {
        return true;
    } else {
        return false;
    }
}

static inline bool
tk_lang_c_multiple_comment_tail_p(char *buf)
{
    assert_exit(buf);

    if (TK_STAR == buf[0] && TK_SLASH == buf[1]) {
        return true;
    } else {
        return false;
    }
}

