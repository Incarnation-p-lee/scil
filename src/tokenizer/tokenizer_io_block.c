static inline s_io_block_t *
tkz_io_block_create(void)
{
    s_io_block_t *io_block;

    io_block = dp_malloc(sizeof(*io_block));
    io_block->block_buf = dp_malloc(sizeof(char) * TKZ_IO_BLOCK_SIZE);
    io_block->size = TKZ_IO_BLOCK_SIZE;

    return io_block;
}

static inline bool
tkz_io_block_structure_legal_p(s_io_block_t *io_block)
{
    if (!io_block) {
        return false;
    } else if (!io_block->size) {
        return false;
    } else if (!io_block->block_buf) {
        return false;
    } else {
        return true;
    }
}

static inline void
tkz_io_block_destroy(s_io_block_t *io_block)
{
    assert_exit(tkz_io_block_structure_legal_p(io_block));

    dp_free(io_block->block_buf);
    dp_free(io_block);
}

static inline uint32
tkz_io_block_fill(s_io_block_t *io_block, char *buf)
{
    uint32 index_last;
    uint32 io_block_size;

    assert_exit(buf);
    assert_exit(tkz_io_block_structure_legal_p(io_block));

    io_block_size = tkz_io_block_data_size(buf);

    if (io_block_size > io_block->size) {
        io_block->block_buf = dp_realloc(io_block->block_buf, io_block_size);
        io_block->size = io_block_size;
    }

    dp_memcpy(io_block->block_buf, buf, sizeof(char) * io_block_size);

    index_last = io_block_size - 1;
    io_block->block_buf[index_last] = NULL_CHAR;

    TKZ_IO_BLOCK_PRINT(io_block);
    return io_block_size;
}

/*
 * A io_block indicate a chunk buffer of io_buffer.
 * and that chunk of buffer end with TK_SENTINEL/NULL_CHAR.
 */
static inline void
tkz_io_block_process(s_tkz_lang_t *tkz_lang,
    s_tk_t *tk_head, s_io_block_t *io_block)
{
    assert_exit(tk_structure_legal_p(tk_head));
    assert_exit(tkz_io_block_structure_legal_p(io_block));
    assert_exit(tkz_lang_structure_legal_p(tkz_lang));

    switch (tkz_lang->type) {
        case TKZ_LANG_C:
            tkz_io_block_lang_c_match(tkz_lang, tk_head, io_block);
            break;
        default:
            assert_exit(false);
            break;
    }
}

/*
 * Return the size of buf of io_block data, include sentinel.
 * Example: |a|b|c| |X|Y|Z|, will return 4.
 *                 ^
 *                 |
 *              sentinel or null_char
 */
static inline uint32
tkz_io_block_data_size(char *buf)
{
    char *c;
    uint32 data_size;

    assert_exit(buf);

    c = buf;
    data_size = 0;
    while (*c != NULL_CHAR && *c != TK_SENTINEL) {
        c++;
        data_size++;
    }

    return data_size + 1;
}

static inline void
tkz_io_block_lang_c_match(s_tkz_lang_t *tkz_lang,
    s_tk_t *tk_head, s_io_block_t *io_block)
{
    char *buf;
    uint32 rest_size;
    uint32 match_size;

    assert_exit(tk_structure_legal_p(tk_head));
    assert_exit(tkz_io_block_structure_legal_p(io_block));
    assert_exit(tkz_lang_structure_legal_p(tkz_lang));

    buf = io_block->block_buf;
    rest_size = dp_strlen(buf);

    while (rest_size != 0) {
        TKZ_LANG_C_BUFFER_PRINT(buf);
        match_size = tkz_lang_c_tk_match(tkz_lang, tk_head, buf);

        if (match_size == NFA_SZ_UNMATCH) {
            scil_log_print_and_exit("Cannot detect any token of '%s'.\n", io_block->block_buf);
        } else {
            buf += match_size;
            rest_size -= match_size;
        }
    }
}

