static inline void
tokenizer_file_print(s_tokenizer_file_t *tkz_file_list)
{
    s_tokenizer_file_t *tkz_file_iterator;

    assert_exit(tokenizer_file_structure_legal_p(tkz_file_list));

    tkz_file_iterator = tkz_file_list;

    do {
        tokenizer_file_node_print(tkz_file_iterator);
        tkz_file_iterator = tokenizer_file_next(tkz_file_iterator);
    } while (tkz_file_iterator != tkz_file_list);
}

static inline void
tokenizer_file_node_print(s_tokenizer_file_t *tkz_file_list)
{
    s_token_t *tk_head;
    s_token_t *tk_iterator;
    e_tokenizer_language_type_t tkz_language_type;

    assert_exit(tokenizer_file_structure_legal_p(tkz_file_list));

    dp_printf(">>>>>> token of %s start:\n", tkz_file_list->filename);

    tk_head = tkz_file_list->tk_list;
    tk_iterator = token_list_node_next(tk_head);
    tkz_language_type = tkz_file_list->tkz_language->type;

    while (tk_iterator != tk_head) {
        token_print(tk_iterator, tkz_language_type);
        tk_iterator = token_list_node_next(tk_iterator);
    }

    dp_printf("<<<<<< analysis token end.\n");
}

