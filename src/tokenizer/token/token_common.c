bool
token_structure_legal_p(s_tk_t *token)
{
    return token_structure_legal_ip(token);
}

bool
token_structure_illegal_p(s_tk_t *token)
{
    return token_structure_illegal_ip(token);
}

static inline bool
token_structure_legal_ip(s_tk_t *token)
{
    if (!token) {
        return false;
    } else if (!token->data && token->type != TK_LEX_HEAD) {
        return false;
    } else if (!doubly_linked_list_structure_legal_p(&token->list)) {
        return false;
    } else {
        return true;
    }
}

static inline bool
token_structure_illegal_ip(s_tk_t *token)
{
    return !token_structure_legal_ip(token);
}

s_tk_t *
token_list_node_previous(s_tk_t *token)
{
    if (TOKEN_STRUCTURE_ILLEGAL_P(token)) {
        return PTR_INVALID;
    } else {
        return token_list_node_previous_i(token);
    }

}

static inline void
token_list_insert_before(s_tk_t *token_head, s_tk_t *inserted)
{
    assert_exit(token_structure_legal_p(token_head));
    assert_exit(token_structure_legal_p(inserted));

    doubly_linked_list_insert_before(&token_head->list, &inserted->list);
}

static inline s_tk_t *
token_list_node_previous_i(s_tk_t *token)
{
    assert_exit(token_structure_legal_p(token));

    return CONTAINS_OF(token->list.previous, s_tk_t, list);
}

s_tk_t *
token_list_node_next(s_tk_t *token)
{
    if (TOKEN_STRUCTURE_ILLEGAL_P(token)) {
        return PTR_INVALID;
    } else {
        return token_list_node_next_i(token);
    }
}

static inline s_tk_t *
token_list_node_next_i(s_tk_t *token)
{
    assert_exit(token_structure_legal_p(token));

    return CONTAINS_OF(token->list.next, s_tk_t, list);
}

