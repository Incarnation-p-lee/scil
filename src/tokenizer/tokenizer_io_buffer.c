static inline s_io_buffer_t *
io_buffer_create(void)
{
    s_io_buffer_t *io_buffer;

    io_buffer = dp_malloc(sizeof(*io_buffer));

    io_buffer->index = 0;
    io_buffer->buf[0] = NULL_CHAR;

    return io_buffer;
}

static inline s_tokenizer_io_buffer_t *
tokenizer_io_buffer_create(char *fname)
{
    s_tokenizer_io_buffer_t *tkz_io_buffer;

    assert_exit(fname);

    tkz_io_buffer = dp_malloc(sizeof(*tkz_io_buffer));
    tkz_io_buffer->fd = dp_fopen(fname, "r");

    if (!tkz_io_buffer->fd) {
        scil_log_print_and_exit("*Error* Failed to locate file %s\n", fname);
    } else {
        TOKENIZER_FILE_OPEN_PRINT(fname);
    }

    tkz_io_buffer->primary = io_buffer_create();
    tkz_io_buffer->secondary = io_buffer_create();

    return tkz_io_buffer;
}

static inline void
tokenizer_io_buffer_destroy(s_tokenizer_io_buffer_t *tkz_io_buffer)
{
    assert_exit(tokenizer_io_buffer_structure_legal_p(tkz_io_buffer));

    dp_free(tkz_io_buffer->secondary);
    dp_free(tkz_io_buffer->primary);
    dp_fclose(tkz_io_buffer->fd);
    dp_free(tkz_io_buffer);
}

static inline bool
tokenizer_io_buffer_structure_legal_p(s_tokenizer_io_buffer_t *tkz_io_buffer)
{
    if (!tkz_io_buffer) {
        return false;
    } else if (!tkz_io_buffer->fd) {
        return false;
    } else if (!io_buffer_structure_legal_p(tkz_io_buffer->primary)) {
        return false;
    } else if (!io_buffer_structure_legal_p(tkz_io_buffer->secondary)) {
        return false;
    } else {
        return true;
    }
}

static inline bool
io_buffer_structure_legal_p(s_io_buffer_t *io_buffer)
{
    if (!io_buffer) {
        return false;
    } else if (io_buffer->index > READ_BUF_SIZE) {
        return false;
    } else {
        return true;
    }
}

static inline bool
tokenizer_io_buffer_fill_buffer_p(s_tokenizer_io_buffer_t *tkz_io_buffer,
    e_tokenizer_language_type_t tkz_type)
{
    assert_exit(tokenizer_io_buffer_structure_legal_p(tkz_io_buffer));

    return tokenizer_io_buffer_fill_secondary_buffer_p(tkz_io_buffer, tkz_type);
}

static inline uint32
tokenizer_io_buffer_skip_multiple_comment(s_tokenizer_io_buffer_t *tkz_io_buffer, uint32 index,
    e_tokenizer_language_type_t tkz_type)
{
    uint32 i;
    char *buf;
    s_array_stack_t *stack;

    assert_exit(index < READ_BUF_SIZE);
    assert_exit(tokenizer_io_buffer_structure_legal_p(tkz_io_buffer));
    assert_exit(token_char_multiple_comment_head_p(tkz_io_buffer->primary->buf + index, tkz_type));

    i = index + 2;
    buf = tkz_io_buffer->primary->buf;
    stack = array_stack_create();
    array_stack_push(stack, stack);

    while (!array_stack_empty_p(stack)) {
        if (token_char_multiple_comment_head_p(buf + i, tkz_type)) {
            array_stack_push(stack, stack);
            i += 2;
        } else if (token_char_multiple_comment_tail_p(buf + i, tkz_type)) {
            array_stack_pop(stack);
            i += 2;
        } else {
            tkz_io_buffer->primary->index = i;
            if (tokenizer_io_buffer_reach_limit_p(tkz_io_buffer->primary)) {
                if (tokenizer_io_buffer_fill_primary_buffer_p(tkz_io_buffer)) {
                    i = tkz_io_buffer->primary->index;
                } else {
                    scil_log_print_and_exit("Unmatched multiple comments pair.\n");
                }
            } else {
                i++;
            }
        }
    }

    array_stack_destroy(&stack);
    return i;
}

static inline uint32
tokenizer_io_buffer_skip_comment(s_tokenizer_io_buffer_t *tkz_io_buffer, uint32 index,
    e_tokenizer_language_type_t tkz_type)
{
    assert_exit(index < READ_BUF_SIZE);
    assert_exit(tokenizer_io_buffer_structure_legal_p(tkz_io_buffer));
    assert_exit(token_char_comment_p(tkz_io_buffer->primary->buf + index, tkz_type));

    if (token_char_single_comment_p(tkz_io_buffer->primary->buf + index, tkz_type)) {
        return tokenizer_io_buffer_skip_single_comment(tkz_io_buffer, index, tkz_type);
    } else {
        return tokenizer_io_buffer_skip_multiple_comment(tkz_io_buffer, index, tkz_type);
    }
}

static inline uint32
tokenizer_io_buffer_skip_single_comment(s_tokenizer_io_buffer_t *tkz_io_buffer, uint32 index,
    e_tokenizer_language_type_t tkz_type)
{
    uint32 i;
    s_io_buffer_t *primary;

    assert_exit(index < READ_BUF_SIZE);
    assert_exit(tokenizer_io_buffer_structure_legal_p(tkz_io_buffer));

    // Fix-Me
    if (!token_char_single_comment_p(tkz_io_buffer->primary->buf + index, tkz_type)) {
        return 0;
    }

    i = index;
    primary = tkz_io_buffer->primary;

    while (TK_NEWLINE != primary->buf[i]) {
        primary->index = i;
        if (tokenizer_io_buffer_reach_limit_p(primary)) {
            if (tokenizer_io_buffer_fill_primary_buffer_p(tkz_io_buffer)) {
                i = primary->index;
            } else {
                return i;
            }
        } else {
            i++;
        }
    }

    assert_exit(i + 1 < READ_BUF_SIZE);
    return i + 1;
}

static inline uint32
tokenizer_io_secondary_buffer_resume(s_io_buffer_t *secondary)
{
    uint32 index, k;

    assert_exit(io_buffer_structure_legal_p(secondary));

    k = 0;
    index = secondary->size;
    while (secondary->buf[index]) {
        secondary->buf[k++] = secondary->buf[index++];
    }
    assert_exit(index <= READ_BUF_SIZE);

    secondary->buf[k] = TK_SENTINEL;
    secondary->size = k;

    return k;
}

/*
 * Secondary buffer will delete space and comments
 * From primary -> secondary buffer
 */
static inline bool
tokenizer_io_buffer_fill_secondary_buffer_p(s_tokenizer_io_buffer_t *tkz_io_buffer,
    e_tokenizer_language_type_t tkz_type)
{
    char last;
    uint32 index, k;
    s_io_buffer_t *primary, *secondary;

    assert_exit(tokenizer_io_buffer_structure_legal_p(tkz_io_buffer));

    last = NULL_CHAR;
    primary = tkz_io_buffer->primary;
    secondary = tkz_io_buffer->secondary;
    index = tokenizer_io_secondary_buffer_resume(secondary);

    while (tokenizer_io_buffer_fill_primary_buffer_p(tkz_io_buffer)) {
        k = primary->index;
        while (!tokenizer_io_buffer_reach_limit_p(primary)) {
            if (token_char_double_quote_p(primary->buf[k])) {
                secondary->buf[index++] = primary->buf[k++];
                do {
                    secondary->buf[index++] = primary->buf[k];
                } while (!token_char_double_quote_p(primary->buf[k++]));
            } else if (dp_isspace(primary->buf[k])) {
                last = primary->buf[k++];
                secondary->size = index + 1;              // include the sentinel
            } else if (token_char_comment_p(primary->buf + k, tkz_type)) {
                k = tokenizer_io_buffer_skip_comment(tkz_io_buffer, k, tkz_type);
            } else if (index < READ_BUF_INDEX_LAST - 1) { // index may +2
                if (dp_isspace(last) && index != 0) {
                    secondary->buf[index++] = TK_SENTINEL;
                }
                secondary->buf[index++] = last = primary->buf[k++];
            } else {
                primary->index = k;
                secondary->buf[index] = NULL_CHAR;
                IO_BUFFER_PRINT(tkz_io_buffer->secondary);
                return true;
            }
            primary->index = k;
        }
    }

    if (index == 0) {
        return false;
    } else { // Make the rest part of buffer begin with NULL_CHAR
        secondary->buf[index] = secondary->buf[index + 1] = NULL_CHAR;
        IO_BUFFER_PRINT(tkz_io_buffer->secondary);
        return true;
    }
}

static inline bool
tokenizer_io_buffer_reach_limit_p(s_io_buffer_t *buffer)
{
    assert_exit(io_buffer_structure_legal_p(buffer));

    if (READ_BUF_SIZE == buffer->index
        || NULL_CHAR == buffer->buf[buffer->index]) {
        return true;
    } else {
        return false;
    }
}

static inline bool
tokenizer_io_buffer_fill_primary_buffer_p(s_tokenizer_io_buffer_t *tkz_io_buffer)
{
    uint32 len;
    uint32 rest;
    uint32 offset;

    assert_exit(tokenizer_io_buffer_structure_legal_p(tkz_io_buffer));

    if (!tokenizer_io_buffer_reach_limit_p(tkz_io_buffer->primary)) {
        return true;
    } else if (dp_feof(tkz_io_buffer->fd)) {
        return false;
    } else {
        tkz_io_buffer->primary->index = offset = 0;
        rest = READ_BUF_SIZE;

        while (rest != 0 && !dp_feof(tkz_io_buffer->fd)) {
            len = dp_fread(tkz_io_buffer->primary->buf + offset, 1, rest, tkz_io_buffer->fd);
            assert_exit(len <= rest);
            offset += len;
            rest -= len;
        }

        tkz_io_buffer->primary->buf[offset] = NULL_CHAR;
        return true;
    }
}

