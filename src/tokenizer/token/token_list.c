s_tk_t *
tk_list_node_previous(s_tk_t *token)
{
    if (TK_STRUCTURE_ILLEGAL_P(token)) {
        return PTR_INVALID;
    } else {
        return tk_list_node_previous_i(token);
    }
}

static inline void
tk_list_insert_before(s_tk_t *tk_head, s_tk_t *inserted)
{
    assert_exit(tk_structure_legal_p(tk_head));
    assert_exit(tk_structure_legal_p(inserted));

    doubly_linked_list_insert_before(&tk_head->list, &inserted->list);
}

static inline s_tk_t *
tk_list_node_previous_i(s_tk_t *token)
{
    assert_exit(tk_structure_legal_p(token));

    return CONTAINS_OF(token->list.previous, s_tk_t, list);
}

s_tk_t *
tk_list_node_next(s_tk_t *token)
{
    if (TK_STRUCTURE_ILLEGAL_P(token)) {
        return PTR_INVALID;
    } else {
        return tk_list_node_next_i(token);
    }
}

static inline s_tk_t *
tk_list_node_next_i(s_tk_t *token)
{
    assert_exit(tk_structure_legal_p(token));

    return CONTAINS_OF(token->list.next, s_tk_t, list);
}

static inline void
tk_list_node_destroy(s_tk_t *tk_node)
{
     s_tk_lang_c_idtr_t *tk_idtr;
     s_tk_lang_c_optr_t *tk_optr;
     s_tk_lang_c_cnst_t *tk_cnst;

     assert_exit(tk_node);

     switch (tk_node->type) {
         case TK_LEX_HEAD:
             dp_free(tk_node->data);
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
tk_list_destroy(s_tk_t *tk_list)
{
    s_tk_t *tk_node;
    s_tk_t *tk_next;

    if (TK_STRUCTURE_LEGAL_P(tk_list)) {
        tk_node = tk_list;
        do {
            tk_next = tk_list_node_next_i(tk_node);
            tk_list_node_destroy(tk_node);
            tk_node = tk_next;
        } while (tk_list != tk_node);
    }
}

