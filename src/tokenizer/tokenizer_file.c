static inline void
tokenizer_file_process(char **file_list, uint32 count)
{
    char **fname;
    s_tokenizer_file_t *tkz_file;

    assert_exit(file_list && count);

    fname = file_list;
    while (fname < file_list + count) {
        tkz_file = tokenizer_file_create(*fname);
        tokenizer_file_token_process(tkz_file);

        tokenizer_file_print(tkz_file);
        tokenizer_file_destroy(tkz_file);
        fname++;
    }

    tokenizer_language_cache_cleanup();
}

static inline s_tokenizer_file_t *
tokenizer_file_create(char *fname)
{
    s_tokenizer_file_t *tkz_file;

    assert_exit(fname);

    tkz_file = dp_malloc(sizeof(*tkz_file));

    tkz_file->filename = dp_malloc(dp_strlen(fname) + 1);
    dp_strcpy(tkz_file->filename, fname);

    tkz_file->tk_list = dp_malloc(sizeof(s_token_t));
    tkz_file->tk_list->data = NULL;
    tkz_file->tk_list->type = TK_LEX_HEAD;
    doubly_linked_list_initial(&tkz_file->tk_list->list);

    tkz_file->tkz_language = tokenizer_language_obtain(fname);
    tkz_file->tkz_io_buffer = tokenizer_io_buffer_create(fname);

    return tkz_file;
}

static inline void
tokenizer_file_destroy(s_tokenizer_file_t *tkz_file)
{
    assert_exit(tokenizer_file_structure_legal_p(tkz_file));

    // No need to free static data for now
    tkz_file->tkz_language = NULL;
    tokenizer_io_buffer_destroy(tkz_file->tkz_io_buffer);
    token_language_c_destroy(tkz_file->tk_list);

    dp_free(tkz_file->filename);
    dp_free(tkz_file);
}

static inline bool
tokenizer_file_structure_legal_p(s_tokenizer_file_t *tkz_file_list)
{
    if (!tkz_file_list) {
        return false;
    } else if (!tkz_file_list->filename) {
        return false;
    } else {
        return true;
    }
}

static inline void
tokenizer_file_token_process(s_tokenizer_file_t *tkz_file)
{
    s_token_t *tk_head;
    s_io_buffer_t *io_buffer;
    s_tokenizer_language_t *tkz_language;
    s_tokenizer_io_buffer_t *tkz_io_buffer;
    e_tokenizer_language_type_t tkz_language_type;

    assert_exit(tokenizer_file_structure_legal_p(tkz_file));

    tk_head = tkz_file->tk_list;
    tkz_language = tkz_file->tkz_language;
    tkz_io_buffer = tkz_file->tkz_io_buffer;

    io_buffer = tkz_io_buffer->secondary;
    tkz_language_type = tkz_language->type;

    while (tokenizer_io_buffer_fill_buffer_p(tkz_io_buffer, tkz_language_type)) {
        tokenizer_file_io_buffer_process(io_buffer, tkz_language, tk_head);
    }
}

static inline void
tokenizer_file_io_buffer_process(s_io_buffer_t *io_buffer,
    s_tokenizer_language_t *tkz_language, s_token_t *token_head)
{
    char *c;
    uint32 limit;
    s_io_block_t *io_block;

    assert_exit(token_head);
    assert_exit(tokenizer_language_structure_legal_p(tkz_language));
    assert_exit(io_buffer_structure_legal_p(io_buffer));

    c = io_buffer->buf;
    limit = io_buffer->size;
    io_block = tokenizer_io_block_create();

    while (c < limit + io_buffer->buf) {
        c += tokenizer_io_block_fill(io_block, c);
        tokenizer_io_block_process(tkz_language, token_head, io_block);
    }
    assert_exit(c == limit + io_buffer->buf);

    tokenizer_io_block_destroy(io_block);
}

