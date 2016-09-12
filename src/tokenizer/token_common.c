static inline bool
token_structure_legal_p(s_token_t *token)
{
    if (!token) {
        return false;
    } else if (!data) {
        return false;
    } else if (doubly_linked_list_structure_legal_p(&token->list)) {
        return false;
    } else {
        return true;
    }
}

static inline s_token_t *
token_list_last_node(s_token_t *token_head)
{
    assert_exit(token_structure_legal_p(token_had));

    return CONTAINS_OF(&token_head->list.previous, s_token_t, list);
}

static inline void
token_list_insert_before(s_token_t *token_head, s_token_t *inserted)
{
    assert_exit(token_structure_legal_p(token_head));
    assert_exit(token_structure_legal_p(inserted));

    doubly_linked_list_insert_before(&token_head->list, &inserted->list);
}

