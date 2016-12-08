static inline s_io_buffer_t *
io_buf_create(void)
{
    s_io_buffer_t *io_buf;

    io_buf = dp_malloc(sizeof(*io_buf));

    io_buf->index = io_buf->size = 0;
    io_buf->buf[0] = NULL_CHAR;

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

static inline char *
tkz_io_buf_secondary_buf(s_tkz_io_buffer_t *tkz_io_buf)
{
    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));

    return tkz_io_buf->secondary->buf;
}

static inline char *
tkz_io_buf_secondary_buf_limit(s_tkz_io_buffer_t *tkz_io_buf)
{
    char *buf;
    uint32 limit;

    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));

    buf = tkz_io_buf_secondary_buf(tkz_io_buf);
    limit = tkz_io_buf_secondary_limit(tkz_io_buf);

    return buf + limit;
}

static inline uint32
tkz_io_buf_secondary_limit(s_tkz_io_buffer_t *tkz_io_buf)
{
    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));

    return tkz_io_buf->secondary->index;
}

static inline bool
tkz_io_buf_secondary_loaded_p(s_tkz_io_buffer_t *tkz_io_buf,
    e_tkz_lang_type_t tkz_type)
{
    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));

    return tkz_io_buf_secondary_fill_p(tkz_io_buf, tkz_type);
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
        } else {
            tkz_io_buf_index_advance(tkz_io_buf, 1u);
        }
    }
}

static inline bool
tkz_io_buf_secondary_full_p(s_tkz_io_buffer_t *tkz_io_buf)
{
    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));

    if (tkz_io_buf->secondary->size < READ_BUF_BASE_SIZE) {
        return false;
    } else {
        return true;
    }
}

static inline bool
tkz_io_buf_secondary_empty_p(s_tkz_io_buffer_t *tkz_io_buf)
{
    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));

    if (tkz_io_buf->secondary->size == 0) {
        return true;
    } else {
        return false;
    }
}

static inline void
tkz_io_buf_secondary_char_append(s_tkz_io_buffer_t *tkz_io_buf, char c)
{
    s_io_buffer_t *secondary;

    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));
    assert_exit(!tkz_io_buf_secondary_overflow_p(tkz_io_buf));

    secondary = tkz_io_buf->secondary;

    secondary->buf[secondary->size] = c;
    secondary->size++;
}

static inline void
tkz_io_buf_secondary_char_fill(s_tkz_io_buffer_t *tkz_io_buf)
{
    s_io_buffer_t *primary;
    s_io_buffer_t *secondary;

    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));
    assert_exit(!tkz_io_buf_secondary_overflow_p(tkz_io_buf));

    primary = tkz_io_buf->primary;
    secondary = tkz_io_buf->secondary;

    secondary->buf[secondary->size] = primary->buf[primary->index];
    primary->index++;
    secondary->size++;
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
}

static inline void
tkz_io_buf_skip_comment(s_tkz_io_buffer_t *tkz_io_buf,
    e_tkz_lang_type_t tkz_type)
{
    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));
    assert_exit(tkz_io_buf_comment_p(tkz_io_buf, tkz_type));

    if (tkz_io_buf_single_comment_head_p(tkz_io_buf, tkz_type)) {
        tkz_io_buf_skip_single_comment(tkz_io_buf, tkz_type);
    } else {
        tkz_io_buf_multiple_comment_skip(tkz_io_buf, tkz_type);
    }
}

static inline void
tkz_io_buf_secondary_resume(s_tkz_io_buffer_t *tkz_io_buf)
{
    uint32 i, k, limit;
    s_io_buffer_t *secondary;

    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));

    secondary = tkz_io_buf->secondary;
    i = secondary->index;

    if (secondary->index == secondary->size) {
        secondary->size = 0;
    } else {
        k = 0;
        limit = secondary->size - 1; /* skip last NULL_CHAR */

        while (i < limit) {
            secondary->buf[k] = secondary->buf[i];
            i++;
            k++;
        }

        secondary->size = k;
    }
}

static inline bool
tkz_io_buf_fill_secondary_tail_p(s_tkz_io_buffer_t *tkz_io_buf)
{
    s_io_buffer_t *secondary;

    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));

    secondary = tkz_io_buf->secondary;

    if (secondary->size == 0) {
        return false;
    } else {
        tkz_io_buf_secondary_char_append(tkz_io_buf, NULL_CHAR);

        IO_BUFFER_PRINT(secondary);
        return true;
    }
}

static inline void
tkz_io_buf_secondary_append_with_space(s_tkz_io_buffer_t *tkz_io_buf,
    bool is_last_space)
{
    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));
    assert_exit(!tkz_io_buf_secondary_full_p(tkz_io_buf));

    if (is_last_space && !tkz_io_buf_secondary_empty_p(tkz_io_buf)) {
        tkz_io_buf_secondary_char_append(tkz_io_buf, TK_SENTINEL);
    }

    tkz_io_buf_secondary_char_fill(tkz_io_buf);
}

/*
 * Secondary buffer will delete space and comments
 * From primary -> secondary buffer
 *
 * *HACK*
 * When input char is space, will set index = size + 1.
 * The index points to first char after sentinel char, but sentinel char
 * is not set for now.
 *
 * Meanwhile, the index also equals the data size of all io_block.
 * because the tail of secondary buf may be truncated.
 *
 * Example if current input char is space.
 *         index = size + 1
 *          |
 *          v
 * |a|b|c| |x|y|z|
 *        ^
 *        |
 *       size
 */
static inline bool
tkz_io_buf_secondary_fill_p(s_tkz_io_buffer_t *tkz_io_buf,
    e_tkz_lang_type_t tkz_type)
{
    bool is_last_space;

    assert_exit(tkz_io_buf_structure_legal_p(tkz_io_buf));

    is_last_space = false;
    tkz_io_buf_secondary_resume(tkz_io_buf);

    while (tkz_io_buf_fill_p(tkz_io_buf)) {
        if (tkz_io_buf_secondary_full_p(tkz_io_buf)) {
            break;
        } else if (tkz_io_buf_double_quote_p(tkz_io_buf)) {
            tkz_io_buf->is_string = !tkz_io_buf->is_string;
            tkz_io_buf_secondary_char_fill(tkz_io_buf);
        } else if (tkz_io_buf->is_string) {
            tkz_io_buf_secondary_char_fill(tkz_io_buf);
        } else if (dp_isspace(tkz_io_buf_char_get(tkz_io_buf))) {
            is_last_space = true;
            tkz_io_buf_index_advance(tkz_io_buf, 1);
            tkz_io_buf->secondary->index = tkz_io_buf->secondary->size + 1;
        } else if (tkz_io_buf_comment_p(tkz_io_buf, tkz_type)) {
            tkz_io_buf_skip_comment(tkz_io_buf, tkz_type);
        } else {
            tkz_io_buf_secondary_append_with_space(tkz_io_buf, is_last_space);
            is_last_space = false;
        }
    }

    return tkz_io_buf_fill_secondary_tail_p(tkz_io_buf);
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

