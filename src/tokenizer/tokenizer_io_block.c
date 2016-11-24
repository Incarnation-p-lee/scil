static inline s_io_block_t *
tokenizer_io_block_create(void)
{
    s_io_block_t *io_block;

    io_block = dp_malloc(sizeof(*io_block));
    io_block->block_buf = dp_malloc(sizeof(char) * TKZ_IO_BLOCK_SIZE);
    io_block->iterate_buf = dp_malloc(sizeof(char) * TKZ_IO_BLOCK_SIZE);
    io_block->size = TKZ_IO_BLOCK_SIZE;

    return io_block;
}

static inline bool
tokenizer_io_block_structure_legal_p(s_io_block_t *io_block)
{
    if (!io_block) {
        return false;
    } else if (!io_block->size) {
        return false;
    } else if (!io_block->block_buf) {
        return false;
    } else if (!io_block->iterate_buf) {
        return false;
    } else {
        return true;
    }
}

static inline void
tokenizer_io_block_destroy(s_io_block_t *io_block)
{
    assert_exit(tokenizer_io_block_structure_legal_p(io_block));

    dp_free(io_block->iterate_buf);
    dp_free(io_block->block_buf);
    dp_free(io_block);
}

static inline uint32
tokenizer_io_block_fill(s_io_block_t *io_block, char *buf)
{
    uint32 io_block_size;

    assert_exit(buf);
    assert_exit(tokenizer_io_block_structure_legal_p(io_block));

    io_block_size = tokenizer_io_block_data_size(buf);
    if (io_block_size > io_block->size) {
        io_block->block_buf = dp_realloc(io_block->block_buf, io_block_size);
        io_block->iterate_buf = dp_realloc(io_block->iterate_buf, io_block_size);
        io_block->size = io_block_size;
    }

    dp_memcpy(io_block->block_buf, buf, sizeof(char) * io_block_size);

    TKZ_IO_BLOCK_PRINT(io_block);
    return io_block_size;
}

/*
 * A io_block indicate a chunk buffer of io_buffer.
 * and that chunk of buffer end with TK_SENTINEL/NULL_CHAR.
 */
static inline void
tokenizer_io_block_process(s_tokenizer_language_t *tkz_language,
    s_token_t *token_head, s_io_block_t *io_block)
{
    assert_exit(token_structure_legal_p(token_head));
    assert_exit(tokenizer_io_block_structure_legal_p(io_block));
    assert_exit(tokenizer_language_structure_legal_p(tkz_language));

    switch (tkz_language->type) {
        case TKZ_LANG_C:
            tokenizer_io_block_language_c_match(tkz_language, token_head, io_block);
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
tokenizer_io_block_data_size(char *buf)
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
tokenizer_io_block_language_c_match(s_tokenizer_language_t *tkz_language,
    s_token_t *token_head, s_io_block_t *io_block)
{
    char *buf;
    uint32 i, last_index;
    uint32 data_size, match_size, rest_size;

    assert_exit(token_structure_legal_p(token_head));
    assert_exit(tokenizer_io_block_structure_legal_p(io_block));
    assert_exit(tokenizer_language_structure_legal_p(tkz_language));

    buf = io_block->iterate_buf;
    data_size = tokenizer_io_block_data_size(io_block->block_buf);
    assert_exit(data_size >= 2);

    dp_memcpy(buf, io_block->block_buf, sizeof(char) * data_size);
    buf[data_size - 1] = NULL_CHAR;

    i = match_size = 0;
    rest_size = dp_strlen(buf);
    last_index = rest_size - 1;

    while (rest_size != 0) {
        match_size = tokenizer_language_c_token_match(tkz_language, token_head, buf);
        if (match_size == NFA_SZ_UNMATCH && last_index == 0) {
            scil_log_print_and_exit("Cannot detect any token of '%s'.\n", io_block->block_buf);
        } else if (match_size == 0) {
            buf[last_index--] = NULL_CHAR;
        } else {
            i += match_size;
            rest_size -= match_size;
            last_index = rest_size - 1;

            dp_memcpy(buf, io_block->block_buf + i, rest_size);
            buf[rest_size] = NULL_CHAR;
        }
    }
}

