static inline uint32
token_language_c_operator_match(s_nfa_engine_t *nfa, s_token_t *token_head, char *buf)
{
    uint32 retval;
    s_token_t *token, *token_last;
    s_token_language_c_optr_t *data;
    e_token_language_c_optr_type_t optr_last, optr;

    assert_exit(buf);
    assert_exit(nfa_engine_structure_legal_p(nfa));
    assert_exit(token_structure_legal_p(token_head));

    retval = nfa_engine_token_match(nfa, buf);

    if (!retval) {
        return 0;
    } else {
        assert_exit(retval <= 2);
        optr = (e_token_language_c_optr_type_t)buf[0];

        if (!token_operator_multi_p(tmp)) {
            token = token_language_c_optr_create(tmp);
            token_insert_before(token_head, token);
        } else {
            token_last = token_list_previous_node(token_head);
            optr_last = token_language_c_optr_type_get(token_last);

            if (token_language_c_optr_consist_of_p(optr_last, optr)) {
                token_language_c_optr_type_set(token_last, TK_C_OPTR_DUAL(optr_last, optr));
            } else {
                token = token_language_c_optr_create(tmp);
                token_insert_before(token_head, token);
            }
        }

        return retval;
    }
}

static inline void
token_language_c_keyword_seek(s_token_language_c_kywd_t *keyword_trie, s_token_t *token)
{
    s_token_language_c_idtr_t *data;
    e_token_language_c_kywd_type_t type;

    assert_exit(token_structure_legal_p(token));
    assert_exit(token_language_c_keyword_structure_legal_p(keyword_trie));

    data = token->data;
    type = token_language_c_keyword_match(keyword_tire, data->name);

    if (TK_C_IDTR_NONE != type) {
        dp_free(data->name);

        data->is_keyword = true;
        data->type = type;
    }
}

static inline uint32
token_language_c_identifier_match(s_nfa_engine_t *nfa, s_token_t *token_head, char *buf)
{
    uint32 retval;
    s_token_t *token;

    assert_exit(buf);
    assert_exit(nfa_engine_structure_legal_p(nfa));
    assert_exit(token_structure_legal_p(token_head));

    retval = nfa_engine_token_match(nfa, buf);

    if (!retval) {
        return 0;
    } else {
        token = token_language_c_idtr_create(buf, retval);
        token_insert_before(token_head, token);

        return retval;
    }
}

static inline uint32
token_language_c_constant_match(s_nfa_engine_t *nfa, s_token_t *token_head, char *buf)
{
    uint32 retval;
    s_token_t *token;

    assert_exit(buf);
    assert_exit(nfa_engine_structure_legal_p(nfa));
    assert_exit(token_structure_legal_p(token_head));

    retval = nfa_engine_token_match(nfa, buf);

    if (!retval) {
        return 0;
    } else {
        token = token_language_c_cnst_create(buf, retval);
        token_insert_before(token_head, token);

        return retval;
    }
}

static inline uint32
token_language_c_punctuation_match(s_nfa_engine_t *nfa, s_token_t *token_head, char *buf)
{
    uint32 retval;
    s_token_t *token;

    assert_exit(buf);
    assert_exit(nfa_engine_structure_legal_p(nfa));
    assert_exit(token_structure_legal_p(token_head));

    retval = nfa_engine_token_match(nfa, buf);

    if (!retval) {
        return 0;
    } else {
        assert_exit(3 > retval);

        token = token_language_c_pctt_create(buf[0]);
        token_insert_before(token_head, token);

        return retval;
    }
}

static inline bool
token_language_c_optr_consist_of_p(e_token_language_c_optr_type_t prefix,
    e_token_language_c_optr_type_t suffix)
{
    assert_exit(token_language_c_optr_type_p(prefix));
    assert_exit(token_language_c_optr_type_p(suffix));

    return token_language_c_optr_dual_consist_of_p(prefix, suffix)
        || token_language_c_optr_triple_prefix(prefix, suffix);
}

static inline bool
token_language_c_optr_dual_consist_of_p(e_token_language_c_optr_type_t prefix,
    e_token_language_c_optr_type_t suffix)
{
    assert_exit(token_language_c_optr_type_p(prefix));
    assert_exit(token_language_c_optr_type_p(suffix));

    return token_language_c_optr_dual_prefix_p(prefix) && token_language_c_optr_dual_suffix_p(suffix);
}

static inline bool
token_language_c_optr_triple_prefix(e_token_language_c_optr_type_t t)
{
    assert_exit(token_language_c_optr_type_p(t));

    switch (t) {
        case TK_C_OPTR_SHT_L:
        case TK_C_OPTR_SHT_R:
            return true;
        default:
            return false;
    }
}

static inline bool
token_language_c_optr_triple_consist_of_p(e_token_language_c_optr_type_t prefix,
    e_token_language_c_optr_type_t suffix)
{
    assert_exit(token_language_c_optr_type_p(prefix));
    assert_exit(token_language_c_optr_type_p(suffix));

    return token_language_c_optr_triple_prefix(prefix) && (TK_C_OPTR_EQ == suffix);
}

static inline bool
token_language_c_optr_triple_prefix_p(e_token_language_c_optr_type_t t)
{
    assert_exit(token_language_c_optr_type_p(t));

    switch (t) {
        case TK_C_OPTR_SHT_L:
        case TK_C_OPTR_SHT_R:
            return true;
        default:
            return false;
    }
}

static inline bool
token_language_c_optr_dual_suffix_p(e_token_language_c_optr_type_t t)
{
    assert_exit(token_language_c_optr_type_p(t));

    switch (t) {
        case TK_C_OPTR_GT:
        case TK_C_OPTR_LT:
        case TK_C_OPTR_EQ:
            return true;
        default:
            return false;
    }
}

static inline bool
token_language_c_optr_dual_prefix_p(e_token_language_c_optr_type_t t)
{
    assert_exit(token_language_c_optr_type_p(t));

    switch (t) {
        case TK_C_OPTR_NOT:
        case TK_C_OPTR_AND:
        case TK_C_OPTR_MUL:
        case TK_C_OPTR_ADD:
        case TK_C_OPTR_SUB:
        case TK_C_OPTR_DIV:
        case TK_C_OPTR_LT:
        case TK_C_OPTR_GT:
        case TK_C_OPTR_XOR:
        case TK_C_OPTR_OR:
            return true;
        default:
            return false;
    }
}

static inline bool
token_language_c_optr_type_p(char t)
{
    switch (t) {
        case TK_C_OPTR_NOT:
        case TK_C_OPTR_MOD:
        case TK_C_OPTR_AND:
        case TK_C_OPTR_MUL:
        case TK_C_OPTR_ADD:
        case TK_C_OPTR_SUB:
        case TK_C_OPTR_REF:
        case TK_C_OPTR_DIV:
        case TK_C_OPTR_CLN:
        case TK_C_OPTR_LT:
        case TK_C_OPTR_EQ:
        case TK_C_OPTR_GT:
        case TK_C_OPTR_QST:
        case TK_C_OPTR_XOR:
        case TK_C_OPTR_OR:
        case TK_C_OPTR_NEG:
            return true;
        default:
            return false;
    }
}

static inline e_token_language_c_optr_type_t
token_language_c_optr_type_get(s_token_t *token)
{
    s_token_language_c_optr_t *data;

    assert_exit(token_structure_legal_p(token));

    data = token->data;

    assert_exit(token_language_c_optr_type_p(data->type));
    return data->type;
}

static inline void
token_language_c_optr_type_set(s_token_t *token, e_token_language_c_optr_type_t type)
{
    s_token_language_c_optr_t *data;

    assert_exit(token_language_c_optr_type_p(type));
    assert_exit(token_structure_legal_p(token));

    data = token->data;
    data->type = type;
}

static inline bool
token_language_c_pctt_type_p(e_token_language_c_pctt_type_t type)
{
    switch (type) {
        case TK_C_PCTT_COMMA:
        case TK_C_PCTT_S_BRKT_L:
        case TK_C_PCTT_S_BRKT_R:
        case TK_C_PCTT_M_BRKT_L:
        case TK_C_PCTT_M_BRKT_L:
        case TK_C_PCTT_L_BRKT_R:
        case TK_C_PCTT_L_BRKT_R:
        case TK_C_PCTT_SEMICOLON:
            return true;
        default:
            return false;
    }
}

static inline s_token_t *
token_language_c_optr_create(e_token_language_c_optr_type_t type)
{
    s_token_t *token;
    s_token_language_c_optr_t *optr;

    assert_exit(token_language_c_optr_type_p(type));

    token = dp_malloc(sizeof(*token));
    token->type = TK_LEX_OPTR;
    token->data = optr = dp_malloc(sizeof(s_token_language_c_optr_t));
    optr->type = type;

    doubly_linked_list_initial(&token->list);
    return token;
}

static inline s_token_t *
token_language_c_idtr_create(char *buf, uint32 size)
{
    char *c;
    s_token_t *token;
    s_token_language_c_idtr_t *idtr;

    assert_exit(buf);

    if (SENTINEL_CHAR != buf[size - 1]) {
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
    return token;
}

static inline s_token_t *
token_language_c_cnst_create(char *buf, uint32 size)
{
    char *c;
    s_token_t *token;
    s_token_language_c_cnst_t *cnst;

    assert_exit(buf)

    if (SENTINEL_CHAR != buf[size - 1]) {
        size++;
    }

    token = dp_malloc(sizeof(*token));
    token->type = TK_LEX_CNST;
    token->data = cnst = dp_malloc(sizeof(s_token_language_c_cnst_t));

    cnst->name = dp_malloc(sizeof(*cnst->name) * size);
    dp_memcpy(cnst->name, buf, sizeof(*name->name) * size);
    cnst->name[size - 1] = NULL_CHAR;

    doubly_linked_list_initial(&token->list);
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
    return token;
}

static inline e_token_language_c_kywd_type_t
token_language_c_keyword_to_type(char **keyword)
{
    e_token_language_c_kywd_type_t type;

    assert_exit(keyword);

    type = (e_token_language_c_kywd_type_t)(keyword - (char **)token_language_c_keywords);

    assert_exit(TK_C_IDTR_FIRST <= type && TK_C_IDTR_LAST > type);
    return type;
}

static inline bool
token_language_c_keyword_structure_legal_p(s_token_language_c_kywd_t *node)
{
    if (!node) {
        return false;
    } else if (NULL_CHAR != node->c && !regular_char_data_p(node->c)) {
        return false;
    } else if (TK_C_IDTR_FIRST > node->type || TK_C_IDTR_LAST < node->type) {
        return false;
    } else {
        return true;
    }
}

static inline s_token_language_c_kywd_t *
token_language_c_keyword_trie_node_create(char c)
{
    s_token_language_c_kywd_t *node;

    assert_exit(NULL_CHAR == c || regular_char_data_p(c));

    node = dp_malloc(sizeof(*node));
    node->c = c;
    node->type = TK_C_IDTR_NONE;
    dp_memset(node->children, 0, sizeof(node->children));

    assert_exit(token_language_c_keyword_structure_legal_p(node));
    return node;
}

s_token_language_c_kywd_t *
token_language_c_keyword_trie_create(void)
{
    char **tmp, *keyword;
    s_token_language_c_kywd_t *keyword_root;

    tmp = token_language_c_keywords;
    keyword_root = token_language_c_keyword_trie_node_create(NULL_CHAR);

    while (tmp < token_language_c_keywords + ARRAY_SIZE_OF(token_language_c_keywords)) {
        token_language_c_keyword_trie_insert(keyword_root, tmp);
        tmp++;
    }

    assert_exit(token_language_c_keyword_trie_legal_p(keyword_root));
    return keyword_root;
}

static inline void
token_language_c_keyword_trie_insert(s_token_language_c_kywd_t *root, char **keyword)
{
    char *c;
    s_token_language_c_kywd_t *node, *node_tmp;
    s_token_language_c_kywd_t **child;

    assert_exit(keyword && *keyword);
    assert_exit(token_language_c_keyword_structure_legal_p(root));

    c = *keyword;
    node = root;
    while (*c) {
        child = node->children;
        while (*child) {
            node_tmp = *child;
            if (node_tmp->c == *c) {
                node = node_tmp;
                break;
            }
            child++;
        }

        if (!*child) {
            *child = token_language_c_keyword_trie_node_create(*c);
        }
        c++;
    }

    (*child)->type = token_language_c_keyword_to_type(keyword);
}

void
token_language_c_keyword_trie_destroy(s_token_language_c_kywd_t *keyword_trie)
{
    s_array_queue_t *queue;
    s_token_language_c_kywd_t *tmp, **child;

    if (token_language_c_keyword_structure_legal_p(keyword_trie)) {
        queue = array_queue_create();
        array_queue_enter(queue, keyword_trie);

        while (!array_queue_empty_p(queue)) {
            tmp = array_queue_leave(queue);
            child = tmp->children;
            while (*child) {
                array_queue_enter(queue, *child);
                child++;
            }
            assert_exit(child <= tmp->children + TOKEN_LANG_SUB_SIZE);

            dp_free(tmp);
        }

        array_queue_destroy(&queue);
    }
}

static inline bool
token_language_c_keyword_trie_node_leaf_p(s_token_language_c_kywd_t *node)
{
    assert_exit(token_language_c_keyword_structure_legal_p(node));

    return NULL == node->children[0];
}

static inline e_token_language_c_kywd_type_t
token_language_c_keyword_match(s_token_language_c_kywd_t *keyword_trie,
    char *idtr)
{
    char *c;
    s_token_language_c_kywd_t **child, *root;

    assert_exit(idtr);
    assert_exit(token_language_c_keyword_structure_legal_p(keyword_trie));

    c = idtr;
    root = keyword_trie;

    while (*c) {
        child = root->children;
        while (*child) {
            if ((*child)->c == *c) {
                root = *child;
                break;
            }
            child++;
        }
        assert_exit(child <= root->children + TOKEN_LANG_SUB_SIZE);

        if (!*child) {
            return TK_C_IDTR_NONE;
        }
    }

    if (!token_language_c_keyword_trie_node_leaf_p(*child)) {
        return TK_C_IDTR_NONE;
    } else {
        assert_exit(TK_C_IDTR_NONE != (*child)->type);
        return (*child)->type;
    }
}

static inline void
token_language_c_keyword_trie_legal_p(s_token_language_c_kywd_t *keyword_trie)
{
    char **tmp;
    s_token_language_c_kywd_t *root;

    assert_exit(token_language_c_keyword_structure_legal_p(keyword_trie));

    root = keyword_trie;
    tmp = token_language_c_keywords;

    while (tmp < token_language_c_keywords + ARRAY_SIZE_OF(token_language_c_keywords)) {
        if (TK_C_IDTR_NONE == token_language_c_keyword_match(root, *tmp)) {
            return false;
        }
        tmp++;
    }

    return true;
}

