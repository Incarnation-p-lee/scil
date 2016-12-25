bool
tk_structure_legal_p(s_tk_t *token)
{
    return tk_structure_legal_ip(token);
}

bool
tk_structure_illegal_p(s_tk_t *token)
{
    return tk_structure_illegal_ip(token);
}

static inline bool
tk_structure_legal_ip(s_tk_t *token)
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
tk_structure_illegal_ip(s_tk_t *token)
{
    return !tk_structure_legal_ip(token);
}

