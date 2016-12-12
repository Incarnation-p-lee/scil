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

    assert_exit(tkz_file_structure_legal_p(tkz_file));

    tk_head = tkz_file->tk_list;
    tkz_lang = tkz_file->tkz_language;
    tkz_io_buf = tkz_file->tkz_io_buffer;

    tkz_file_tk_io_buffer_process(tkz_io_buf, tkz_lang, tk_head);
}

static inline void
tkz_file_tk_io_buffer_process(s_tkz_io_buffer_t *tkz_io_buf,
    s_tkz_lang_t *tkz_lang, s_tk_t *tk_head)
{
    bool is_string;
    s_io_block_t *io_block;

    assert_exit(tk_structure_legal_p(tk_head));
    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));

    is_string = false;
    io_block = tkz_io_block_create();

    while (tkz_io_buf_fill_p(tkz_io_buf)) {
        if (tkz_io_buf_double_quote_p(tkz_io_buf)) {
            is_string = !is_string;
            tkz_io_block_char_fill(io_block, tkz_io_buf);
        } else if (is_string) {
            tkz_io_block_char_fill(io_block, tkz_io_buf);
        } else if (tkz_io_buf_comment_p(tkz_io_buf, tkz_lang->type)) {
            tkz_io_buf_skip_comment(tkz_io_buf, tkz_lang->type);
        } else if (!tkz_io_buf_char_space_p(tkz_io_buf)) {
            tkz_io_block_char_fill(io_block, tkz_io_buf);
        } else {
            tkz_io_block_fill(io_block, tkz_io_buf);
            tkz_file_tk_io_block_process(io_block, tk_head, tkz_lang);
        }
    }

    tkz_io_block_destroy(io_block);
}

static inline void
tkz_file_tk_io_block_process(s_io_block_t *io_block, s_tk_t *tk_head,
    s_tkz_lang_t *tkz_lang)
{
    assert_exit(tk_structure_legal_p(tk_head));
    assert_exit(tkz_io_block_structure_legal_p(io_block));

    RETURN_IF_TRUE(tkz_io_block_empty_p(io_block));

    switch (tkz_lang->type) {
        case TKZ_LANG_C:
            tkz_io_block_lang_c_match(tkz_lang, tk_head, io_block);
            break;
        default:
            assert_exit(false);
            break;
    }

    tkz_io_block_cleanup(io_block);
}

