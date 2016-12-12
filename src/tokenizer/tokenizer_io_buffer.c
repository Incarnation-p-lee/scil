static inline s_io_buffer_t *
io_buf_create(void)
{
    s_io_buffer_t *io_buf;

    io_buf = dp_malloc(sizeof(*io_buf));

    io_buf->buf[0] = NULL_CHAR;
    io_buf->index = io_buf->size = 0;

    return io_buf;
}

static inline s_tkz_io_buffer_t *
tkz_io_buf_create(char *fname)
{
    s_tkz_io_buffer_t *tkz_io_buf;

    assert_exit(fname);

    tkz_io_buf = dp_malloc(sizeof(*tkz_io_buf));
    tkz_io_buf->fd = dp_fopen(fname, "r");

    if (!tkz_io_buf->fd) {
        log_print_and_exit("*Error* Failed to locate file %s\n", fname);
    } else {
        TKZ_FILE_OPEN_PRINT(fname);
    }

    tkz_io_buf->line_nmbr = 1;
    tkz_io_buf->is_string = false;

    tkz_io_buf->primary = io_buf_create();
    tkz_io_buf->secondary = io_buf_create();

    return tkz_io_buf;
}

static inline void
tkz_io_buf_destroy(s_tkz_io_buffer_t *tkz_io_buf)
{
    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));

    dp_free(tkz_io_buf->secondary);
    dp_free(tkz_io_buf->primary);
    dp_fclose(tkz_io_buf->fd);
    dp_free(tkz_io_buf);
}

static inline void
tkz_io_buf_index_advance(s_tkz_io_buffer_t *tkz_io_buf, uint32 count)
{
    uint32 rest;

    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));

    rest = count;
    while (rest != 0) {
        if (tkz_io_buf_fill_p(tkz_io_buf)) {
            rest--;
            tkz_io_buf->primary->index++;
        } else {
            log_print_and_exit("Reach the end of file.\n");
        }
    }
}

static inline char *
tkz_io_buf_current(s_tkz_io_buffer_t *tkz_io_buf)
{
    char *buf;

    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));

    buf = tkz_io_buf->primary->buf;
    buf += tkz_io_buf->primary->index;

    return buf;
}

static inline bool
tkz_io_buf_single_comment_head_p(s_tkz_io_buffer_t *tkz_io_buf,
    e_tkz_lang_type_t tkz_type)
{
    char *buf;

    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));

    buf = tkz_io_buf_current(tkz_io_buf);

    return tk_char_single_comment_p(buf, tkz_type);
}

static inline char
tkz_io_buf_single_comment_end(e_tkz_lang_type_t tkz_type)
{
    return tk_char_single_comment_end(tkz_type);
}

static inline bool
tkz_io_buf_multiple_comment_head_p(s_tkz_io_buffer_t *tkz_io_buf,
    e_tkz_lang_type_t tkz_type)
{
    char *buf;

    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));

    buf = tkz_io_buf_current(tkz_io_buf);

    return tk_char_multiple_comment_head_p(buf, tkz_type);
}

static inline bool
tkz_io_buf_multiple_comment_tail_p(s_tkz_io_buffer_t *tkz_io_buf,
    e_tkz_lang_type_t tkz_type)
{
    char *buf;

    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));

    buf = tkz_io_buf_current(tkz_io_buf);

    return tk_char_multiple_comment_tail_p(buf, tkz_type);
}

static inline bool
tkz_io_buf_comment_p(s_tkz_io_buffer_t *tkz_io_buf,
    e_tkz_lang_type_t tkz_type)
{
    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));

    return tkz_io_buf_single_comment_head_p(tkz_io_buf, tkz_type)
        || tkz_io_buf_multiple_comment_head_p(tkz_io_buf, tkz_type);
}

static inline char
tkz_io_buf_char_get(s_tkz_io_buffer_t *tkz_io_buf)
{
    char *buf;

    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));

    buf = tkz_io_buf->primary->buf;

    return buf[tkz_io_buf->primary->index];
}

static inline bool
tkz_io_buf_char_space_p(s_tkz_io_buffer_t *tkz_io_buf)
{
    uint32 c;

    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));

    c = tkz_io_buf_char_get(tkz_io_buf);

    return dp_isspace(c);
}

static inline void
tkz_io_buf_multiple_comment_skip(s_tkz_io_buffer_t *tkz_io_buf,
    e_tkz_lang_type_t tkz_type)
{
    sint32 stack;

    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));
    assert_exit(tkz_io_buf_multiple_comment_head_p(tkz_io_buf, tkz_type));

    tkz_io_buf_index_advance(tkz_io_buf, 2u); /* Advance 2 char */

    stack = 1; /* a emulated stack */

    while (stack != 0) {
        if (tkz_io_buf_multiple_comment_head_p(tkz_io_buf, tkz_type)) {
            stack++;
            tkz_io_buf_index_advance(tkz_io_buf, 2u);
        } else if (tkz_io_buf_multiple_comment_tail_p(tkz_io_buf, tkz_type)) {
            stack--;
            tkz_io_buf_index_advance(tkz_io_buf, 2u);
        } else if (tkz_io_buf_char_get(tkz_io_buf) == TK_NEWLINE) {
            tkz_io_buf->line_nmbr++;
            tkz_io_buf_index_advance(tkz_io_buf, 1u);
        } else {
            tkz_io_buf_index_advance(tkz_io_buf, 1u);
        }
    }
}

static inline bool
tkz_io_buf_double_quote_p(s_tkz_io_buffer_t *tkz_io_buf)
{
    char c;

    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));

    c = tkz_io_buf_char_get(tkz_io_buf);

    return tk_char_double_quote_p(c);
}

static inline void
tkz_io_buf_skip_single_comment(s_tkz_io_buffer_t *tkz_io_buf,
    e_tkz_lang_type_t tkz_type)
{
    char comment_end;

    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));
    assert_exit(tkz_io_buf_single_comment_head_p(tkz_io_buf, tkz_type));

    comment_end = tkz_io_buf_single_comment_end(tkz_type);

    while (tkz_io_buf_char_get(tkz_io_buf) != comment_end) {
        tkz_io_buf_index_advance(tkz_io_buf, 1u);
    }

    tkz_io_buf_index_advance(tkz_io_buf, 1u);
}

static inline void
tkz_io_buf_skip_comment(s_tkz_io_buffer_t *tkz_io_buf,
    e_tkz_lang_type_t tkz_type)
{
    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));
    assert_exit(tkz_io_buf_comment_p(tkz_io_buf, tkz_type));

    if (tkz_io_buf_single_comment_head_p(tkz_io_buf, tkz_type)) {
        tkz_io_buf->line_nmbr++;
        tkz_io_buf_skip_single_comment(tkz_io_buf, tkz_type);
    } else {
        tkz_io_buf_multiple_comment_skip(tkz_io_buf, tkz_type);
    }
}

static inline bool
tkz_io_buf_limited_p(s_io_buffer_t *buffer)
{
    assert_exit(io_buf_structure_legal_p(buffer));

    if (buffer->index == buffer->size) {
        return true;
    } else {
        return false;
    }
}

static inline bool
tkz_io_buf_primary_limited_p(s_tkz_io_buffer_t *tkz_io_buf)
{
    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));

    return tkz_io_buf_limited_p(tkz_io_buf->primary);
}

static inline bool
tkz_io_buf_fill_ip(s_tkz_io_buffer_t *tkz_io_buf)
{
    char *buf;
    uint32 len;
    uint32 rest;
    uint32 offset;

    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));

    rest = READ_BUF_BASE_SIZE;
    buf = tkz_io_buf->primary->buf;
    offset = tkz_io_buf->primary->index = 0;

    while (rest != 0 && !dp_feof(tkz_io_buf->fd)) {
        len = dp_fread(buf + offset, 1, rest, tkz_io_buf->fd);
        offset += len;
        rest -= len;
    }

    if (offset == 0) {
        return false; /* no data can be filled */
    }

    if (rest == 0 && buf[offset - 1] == TKZ_LANG_C_COMMENT) {
        dp_fread(buf + offset, 1, 1, tkz_io_buf->fd);;
        offset++;
    }

    buf[offset] = NULL_CHAR;
    tkz_io_buf->primary->size = offset;

    return true;
}

static inline bool
tkz_io_buf_fill_p(s_tkz_io_buffer_t *tkz_io_buf)
{
    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));

    if (!tkz_io_buf_primary_limited_p(tkz_io_buf)) {
        return true;
    } else if (dp_feof(tkz_io_buf->fd)) {
        return false;
    } else {
        return tkz_io_buf_fill_ip(tkz_io_buf);
    }
}

