static inline uint32
token_lang_c_match_operator(s_nfa_engine_t *nfa, s_token_t *token_head,
    char *buf, char sentinel)
{
    uint32 retval;
    s_token_c_optr_t *data;
    s_token_t *token, *token_last;
    e_token_lang_c_optr_t optr_last, optr;

    assert_exit(buf);
    assert_exit(token_head);
    assert_exit(nfa_engine_structure_legal_p(nfa));

    retval = nfa_engine_token_match(nfa, buf, sentinel);

    if (!retval) {
        return 0;
    } else {
        assert_exit(retval <= 2);
        optr = (e_token_lang_c_optr_t)buf[0];

        if (!token_operator_multi_p(tmp)) {
            token = token_lang_c_optr_create(tmp);
            token_insert_before(token_head, token);
        } else {
            token_last = token_list_last_node(token_head);
            optr_last = token_lang_c_optr_type_get(token_last);

            if (token_lang_c_optr_consist_of_p(optr_last, optr)) {
                token_lang_c_optr_type_set(token_last, TK_OPTR_DUAL(optr_last, optr));
            } else {
                token = token_lang_c_optr_create(tmp);
                token_insert_before(token_head, token);
            }
        }

        return retval;
    }
}

static inline bool
token_lang_c_optr_consist_of_p(e_token_lang_c_optr_t prefix,
    e_token_lang_c_optr_t suffix)
{
    assert_exit(token_lang_c_optr_p(prefix));
    assert_exit(token_lang_c_optr_p(suffix));

    return token_lang_c_optr_dual_consist_of_p(prefix, suffix)
        || token_lang_c_optr_triple_prefix(prefix, suffix);
}


static inline bool
token_lang_c_optr_dual_consist_of_p(e_token_lang_c_optr_t prefix,
    e_token_lang_c_optr_t suffix)
{
    assert_exit(token_lang_c_optr_p(prefix));
    assert_exit(token_lang_c_optr_p(suffix));

    return token_lang_c_optr_dual_prefix_p(prefix) && token_lang_c_optr_dual_suffix_p(suffix);
}

static inline bool
token_lang_c_optr_triple_prefix(e_token_lang_c_optr_t t)
{
    assert_exit(token_lang_c_optr_p(t));

    switch (t) {
        case TK_OPTR_SHT_L:
        case TK_OPTR_SHT_R:
            return true;
        default:
            return false;
    }
}

static inline bool
token_lang_c_optr_triple_consist_of_p(e_token_lang_c_optr_t prefix,
    e_token_lang_c_optr_t suffix)
{
    assert_exit(token_lang_c_optr_p(prefix));
    assert_exit(token_lang_c_optr_p(suffix));

    return token_lang_c_optr_triple_prefix(prefix) && (TK_OPTR_EQ == suffix);
}

static inline bool
token_lang_c_optr_triple_prefix_p(e_token_lang_c_optr_t t)
{
    assert_exit(token_lang_c_optr_p(t));

    switch (t) {
        case TK_OPTR_SHT_L:
        case TK_OPTR_SHT_R:
            return true;
        default:
            return false;
    }
}

static inline bool
token_lang_c_optr_dual_suffix_p(e_token_lang_c_optr_t t)
{
    assert_exit(token_lang_c_optr_p(t));

    switch (t) {
        case TK_OPTR_GT:
        case TK_OPTR_LT:
        case TK_OPTR_EQ:
            return true;
        default:
            return false;
    }
}

static inline bool
token_lang_c_optr_dual_prefix_p(e_token_lang_c_optr_t t)
{
    assert_exit(token_lang_c_optr_p(t));

    switch (t) {
        case TK_OPTR_NOT:
        case TK_OPTR_AND:
        case TK_OPTR_MUL:
        case TK_OPTR_ADD:
        case TK_OPTR_SUB:
        case TK_OPTR_DIV:
        case TK_OPTR_LT:
        case TK_OPTR_GT:
        case TK_OPTR_XOR:
        case TK_OPTR_OR:
            return true;
        default:
            return false;
    }
}


static inline bool
token_lang_c_optr_p(char t)
{
    switch (t) {
        case TK_OPTR_NOT:
        case TK_OPTR_MOD:
        case TK_OPTR_AND:
        case TK_OPTR_MUL:
        case TK_OPTR_ADD:
        case TK_OPTR_SUB:
        case TK_OPTR_REF:
        case TK_OPTR_DIV:
        case TK_OPTR_CLN:
        case TK_OPTR_LT:
        case TK_OPTR_EQ:
        case TK_OPTR_GT:
        case TK_OPTR_QST:
        case TK_OPTR_XOR:
        case TK_OPTR_OR:
        case TK_OPTR_NEG:
            return true;
        default:
            return false;
    }
}

static inline e_token_lang_c_optr_t
token_lang_c_optr_type_get(s_token_t *token)
{
    s_token_c_optr_t *data;

    assert_exit(token_structure_legal_p(token));

    data = token->data;

    assert_exit(token_lang_c_optr_p(data->type));
    return data->type;
}

static inline void
token_lang_c_optr_type_set(s_token_t *token, e_token_lang_c_optr_t type)
{
    s_token_c_optr_t *data;

    assert_exit(token_lang_c_optr_p(type));
    assert_exit(token_structure_legal_p(token));

    data = token->data;
    data->type = type;
}

static inline s_token_t *
token_lang_c_optr_create(e_token_lang_c_optr_t type)
{
    s_token_t *token;
    s_token_c_optr_t *optr;

    assert_exit(token_lang_c_optr_p(type));

    token = dp_malloc(sizeof(*token));
    token->data = optr = dp_malloc(sizeof(s_token_c_optr_t));

    token->type = LEX_OPTR;
    doubly_linked_list_initial(&token->list);
    optr->type = type;

    return token;
}

