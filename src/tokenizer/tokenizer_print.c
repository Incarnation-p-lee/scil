static inline void
tokenizer_file_print(s_tokenizer_file_t *tkz_file)
{
    s_tk_t *tk_head;
    s_tk_t *tk_iterator;
    e_tokenizer_language_type_t tkz_language_type;

    assert_exit(tokenizer_file_structure_legal_p(tkz_file));

    dp_printf(">>>>>> tokenizer analysis '%s' start:\n", tkz_file->filename);

    tk_head = tkz_file->tk_list;
    tk_iterator = token_list_node_next(tk_head);
    tkz_language_type = tkz_file->tkz_language->type;

    while (tk_iterator != tk_head) {
        token_print(tk_iterator, tkz_language_type);
        tk_iterator = token_list_node_next(tk_iterator);
    }

    dp_printf("<<<<<< analysis token end.\n\n");
}

