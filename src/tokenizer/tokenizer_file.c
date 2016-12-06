static inline void
tkz_file_process(char **file_list, uint32 count)
{
    char **fname;
    s_tkz_file_t *tkz_file;

    assert_exit(file_list && count);

    fname = file_list;
    while (fname < file_list + count) {
        tkz_file = tkz_file_create(*fname);
        tkz_file_tk_process(tkz_file);

        tkz_file_print(tkz_file);
        tkz_file_destroy(tkz_file);
        fname++;
    }

    tkz_lang_cache_cleanup();
}

static inline s_tkz_file_t *
tkz_file_create(char *fname)
{
    s_tkz_file_t *tkz_file;

    assert_exit(fname);

    tkz_file = dp_malloc(sizeof(*tkz_file));

    tkz_file->filename = dp_malloc(dp_strlen(fname) + 1);
    dp_strcpy(tkz_file->filename, fname);

    tkz_file->tk_list = dp_malloc(sizeof(s_tk_t));
    tkz_file->tk_list->data = NULL;
    tkz_file->tk_list->type = TK_LEX_HEAD;
    doubly_linked_list_initial(&tkz_file->tk_list->list);

    tkz_file->tkz_language = tkz_lang_obtain(fname);
    tkz_file->tkz_io_buffer = tkz_io_buf_create(fname);

    return tkz_file;
}

static inline void
tkz_file_destroy(s_tkz_file_t *tkz_file)
{
    assert_exit(tkz_file_structure_legal_p(tkz_file));

    /* No need to free static data for now */
    tkz_file->tkz_language = NULL;
    tkz_io_buf_destroy(tkz_file->tkz_io_buffer);
    tk_lang_c_destroy(tkz_file->tk_list);

    dp_free(tkz_file->filename);
    dp_free(tkz_file);
}

static inline void
tkz_file_tk_process(s_tkz_file_t *tkz_file)
{
    s_tk_t *tk_head;
    s_tkz_lang_t *tkz_lang;
    s_tkz_io_buffer_t *tkz_io_buf;
    e_tkz_lang_type_t tkz_lang_type;

    assert_exit(tkz_file_structure_legal_p(tkz_file));

    tk_head = tkz_file->tk_list;
    tkz_lang = tkz_file->tkz_language;
    tkz_io_buf = tkz_file->tkz_io_buffer;

    tkz_lang_type = tkz_lang->type;

    while (tkz_io_buf_secondary_loaded_p(tkz_io_buf, tkz_lang_type)) {
        tkz_file_io_buf_process(tkz_io_buf, tkz_lang, tk_head);
    }
}

static inline void
tkz_file_io_buf_process(s_tkz_io_buffer_t *tkz_io_buf,
    s_tkz_lang_t *tkz_lang, s_tk_t *tk_head)
{
    char *c;
    char *limit;
    s_io_block_t *io_block;

    assert_exit(tk_head);
    assert_exit(tkz_lang_structure_legal_p(tkz_lang));
    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));

    io_block = tkz_io_block_create();
    c = tkz_io_buf_secondary_buf(tkz_io_buf);
    limit = tkz_io_buf_secondary_buf_limit(tkz_io_buf);

    while (c < limit) {
        c += tkz_io_block_fill(io_block, c);
        tkz_io_block_process(tkz_lang, tk_head, io_block);
    }

    assert_exit(c == limit);
    tkz_io_block_destroy(io_block);
}

