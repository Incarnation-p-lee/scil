bool
token_structure_legal_p(s_token_t *token)
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

s_token_t *
token_list_node_previous(s_token_t *token)
{
    if (!token_structure_legal_p(token)) {
        return PTR_INVALID;
    } else {
        return token_list_node_previous_i(token);
    }

}

static inline void
token_list_insert_before(s_token_t *token_head, s_token_t *inserted)
{
    assert_exit(token_structure_legal_p(token_head));
    assert_exit(token_structure_legal_p(inserted));

    doubly_linked_list_insert_before(&token_head->list, &inserted->list);
}

static inline s_token_t *
token_list_node_previous_i(s_token_t *token)
{
    assert_exit(token_structure_legal_p(token));

    return CONTAINS_OF(token->list.previous, s_token_t, list);
}

s_token_t *
token_list_node_next(s_token_t *token)
{
    if (!token_structure_legal_p(token)) {
        return PTR_INVALID;
    } else {
        return token_list_node_next_i(token);
    }
}

static inline s_token_t *
token_list_node_next_i(s_token_t *token)
{
    assert_exit(token_structure_legal_p(token));

    return CONTAINS_OF(token->list.next, s_token_t, list);
}

