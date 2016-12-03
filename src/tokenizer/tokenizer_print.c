static inline void
tkz_file_print(s_tkz_file_t *tkz_file)
{
    s_tk_t *tk_head;
    s_tk_t *tk_iterator;
    e_tkz_lang_type_t tkz_lang_type;

    assert_exit(tkz_file_structure_legal_p(tkz_file));

    dp_printf(">>>>>> tokenizer analysis '%s' start:\n", tkz_file->filename);

    tk_head = tkz_file->tk_list;
    tk_iterator = tk_list_node_next(tk_head);
    tkz_lang_type = tkz_file->tkz_language->type;

    while (tk_iterator != tk_head) {
        tk_print(tk_iterator, tkz_lang_type);
        tk_iterator = tk_list_node_next(tk_iterator);
    }

    dp_printf("<<<<<< analysis token end.\n\n");
}

