static inline s_io_block_t *
tkz_io_block_create(void)
{
    s_io_block_t *io_block;

    io_block = dp_malloc(sizeof(*io_block));

    io_block->line_nmbr = 0;
    io_block->index = 0;
    io_block->size = TKZ_IO_BLOCK_SIZE;
    io_block->buf = dp_malloc(sizeof(char) * TKZ_IO_BLOCK_SIZE);

    return io_block;
}

static inline void
tkz_io_block_destroy(s_io_block_t *io_block)
{
    assert_exit(tkz_io_block_structure_legal_p(io_block));

    dp_free(io_block->buf);
    dp_free(io_block);
}

static inline void
tkz_io_block_fill(s_io_block_t *io_block, s_tkz_io_buffer_t *tkz_io_buf)
{
    assert_exit(tkz_io_block_structure_legal_p(io_block));
    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));

    io_block->buf[io_block->index] = NULL_CHAR;
    io_block->line_nmbr = tkz_io_buf->line_nmbr;

    if (tkz_io_buf_char_get(tkz_io_buf) == TK_NEWLINE) {
        tkz_io_buf->line_nmbr++;
    }

    tkz_io_buf_index_advance(tkz_io_buf, 1u);

    TKZ_IO_BLOCK_PRINT(io_block);
}

static inline bool
tkz_io_block_empty_p(s_io_block_t *io_block)
{
    char *buf;

    assert_exit(tkz_io_block_structure_legal_p(io_block));

    buf = io_block->buf;

    if (buf[0] == NULL_CHAR) {
        return true;
    } else {
        return false;
    }
}

static inline void
tkz_io_block_cleanup(s_io_block_t *io_block)
{
    assert_exit(tkz_io_block_structure_legal_p(io_block));

    io_block->index = 0;
}

static inline void
tkz_io_block_char_fill(s_io_block_t *io_block, s_tkz_io_buffer_t *tkz_io_buf)
{
    char c;

    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));
    assert_exit(tkz_io_block_structure_legal_p(io_block));

    c = tkz_io_buf_char_get(tkz_io_buf);
    tkz_io_buf_index_advance(tkz_io_buf, 1u);

    io_block->buf[io_block->index++] = c;

    if (io_block->index == io_block->size) {
        io_block->size = io_block->size * 2;
        io_block->buf = dp_realloc(io_block->buf, io_block->size);
    }
}

static inline void
tkz_io_block_lang_c_match(s_tkz_lang_t *tkz_lang,
    s_tk_t *tk_head, s_io_block_t *io_block)
{
    char *buf;
    s_tk_t *last_tk;
    uint32 rest_size;
    uint32 match_size;

    assert_exit(tk_structure_legal_p(tk_head));
    assert_exit(tkz_io_block_structure_legal_p(io_block));
    assert_exit(tkz_lang_structure_legal_p(tkz_lang));

    buf = io_block->buf;
    rest_size = dp_strlen(buf);

    while (rest_size != 0) {
        TKZ_LANG_C_BUFFER_PRINT(buf);
        match_size = tkz_lang_c_tk_match(tkz_lang, tk_head, buf);

        if (match_size == NFA_SZ_UNMATCH) {
            dp_printf("Cannot detect any token of '%s' at line %d.\n",
                io_block->buf, io_block->line_nmbr);
            log_print_and_exit("Cannot detect any token of '%s'.\n",
                io_block->buf);
        } else {
            buf += match_size;
            rest_size -= match_size;

            last_tk = tk_list_node_previous(tk_head);
            last_tk->line_nmbr = io_block->line_nmbr;
        }
    }
}

