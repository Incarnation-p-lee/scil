static inline s_io_buffer_t *
tokenizer_aim_open_buffer_create(void)
{
    s_io_buffer_t *buf;

    buf = dp_malloc(sizeof(*buf));

    buf->index = 0;
    buf->buf[0] = NULL_CHAR;

    return buf;
}

static inline s_tokenizer_aim_t *
tokenizer_aim_open(char *fname)
{
    s_tokenizer_aim_t *aim;

    assert_exit(fname);

    aim = dp_malloc(sizeof(*aim));
    dp_strcpy(aim->fname, fname);

    aim->fd = dp_fopen(fname, "r");
    if (!aim->fd) {
        scil_log_print_and_exit("*Error* Failed to locate file %s\n", fname);
    } else {
        TOKENIZER_AIM_OPEN_PRINT(fname);
    }

    aim->primary = tokenizer_aim_open_buffer_create();
    aim->secondary = tokenizer_aim_open_buffer_create();

    return aim;
}

static inline void
tokenizer_aim_close(s_tokenizer_aim_t *aim)
{
    assert_exit(tokenizer_aim_structure_legal_p(aim));

    TOKENIZER_AIM_CLOSE_PRINT(aim->fname);

    dp_free(aim->secondary);
    dp_free(aim->primary);
    dp_fclose(aim->fd);
    dp_free(aim);
}

static inline bool
tokenizer_aim_structure_legal_p(s_tokenizer_aim_t *aim)
{
    if (!aim) {
        return false;
    } else if (!aim->fd) {
        return false;
    } else if (!tokenizer_io_buffer_structure_legal_p(aim->primary)) {
        return false;
    } else if (!tokenizer_io_buffer_structure_legal_p(aim->secondary)) {
        return false;
    } else {
        return true;
    }
}

static inline bool
tokenizer_io_buffer_structure_legal_p(s_io_buffer_t *buf)
{
    if (!buf) {
        return false;
    } else if (buf->index > READ_BUF_SIZE) {
        return false;
    } else {
        return true;
    }
}

static inline bool
tokenizer_aim_fill_buffer_p(s_tokenizer_aim_t *aim)
{
    assert_exit(tokenizer_aim_structure_legal_p(aim));

    return tokenizer_aim_fill_secondary_buffer_p(aim);
}

static inline bool
tokenizer_char_single_comment_p(char *buf)
{
    assert_exit(buf);

    if (SLASH_CHAR == buf[0] && SLASH_CHAR == buf[1]) {
        return true;
    } else {
        return false;
    }
}

static inline bool
tokenizer_char_multiple_comment_head_p(char *buf)
{
    assert_exit(buf);

    if (SLASH_CHAR == buf[0] && STAR_CHAR == buf[1]) {
        return true;
    } else {
        return false;
    }
}

static inline bool
tokenizer_char_multiple_comment_tail_p(char *buf)
{
    assert_exit(buf);

    if (STAR_CHAR == buf[0] && SLASH_CHAR == buf[1]) {
        return true;
    } else {
        return false;
    }
}

static inline uint32
tokenizer_aim_skip_multiple_comment(s_tokenizer_aim_t *aim, uint32 index)
{
    uint32 i;
    char *buf;
    s_array_stack_t *stack;

    assert_exit(index < READ_BUF_SIZE);
    assert_exit(tokenizer_aim_structure_legal_p(aim));
    assert_exit(tokenizer_char_multiple_comment_head_p(aim->primary->buf + index));

    i = index + 2;
    buf = aim->primary->buf;
    stack = array_stack_create();
    array_stack_push(stack, stack);

    while (!array_stack_empty_p(stack)) {
        if (tokenizer_char_multiple_comment_head_p(buf + i)) {
            array_stack_push(stack, stack);
            i += 2;
        } else if (tokenizer_char_multiple_comment_tail_p(buf + i)) {
            array_stack_pop(stack);
            i += 2;
        } else {
            aim->primary->index = i;
            if (tokenizer_io_buffer_reach_limit_p(aim->primary)) {
                if (tokenizer_aim_fill_primary_buffer_p(aim)) {
                    i = aim->primary->index;
                } else {
                    scil_log_print_and_exit("Unmatched multiple comments pair.\n");
                    return INDEX_INVALID;
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
tokenizer_aim_skip_single_comment(s_tokenizer_aim_t *aim, uint32 index)
{
    uint32 i;
    s_io_buffer_t *primary;

    assert_exit(index < READ_BUF_SIZE);
    assert_exit(tokenizer_aim_structure_legal_p(aim));
    assert_exit(tokenizer_char_single_comment_p(aim->primary->buf + index));

    i = index;
    primary = aim->primary;

    while (NLINE_CHAR != primary->buf[i]) {
        primary->index = i;
        if (tokenizer_io_buffer_reach_limit_p(primary)) {
            if (tokenizer_aim_fill_primary_buffer_p(aim)) {
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

    assert_exit(tokenizer_io_buffer_structure_legal_p(secondary));

    k = 0;
    index = secondary->size;
    while (secondary->buf[index]) {
        secondary->buf[k++] = secondary->buf[index++];
    }

    assert_exit(index <= READ_BUF_SIZE);
    secondary->buf[k] = NULL_CHAR;
    secondary->size = k;

    return k;
}

/*
 * Secondary buffer will delete space and comments
 * From primary -> secondary buffer
 */
static inline bool
tokenizer_aim_fill_secondary_buffer_p(s_tokenizer_aim_t *aim)
{
    char *buf;
    uint32 index, k, size;
    s_io_buffer_t *primary;
    s_io_buffer_t *secondary;

    assert_exit(tokenizer_aim_structure_legal_p(aim));

    primary = aim->primary;
    secondary = aim->secondary;
    index = tokenizer_io_secondary_buffer_resume(secondary);

    while (tokenizer_aim_fill_primary_buffer_p(aim)) {
        k = primary->index;
        buf = primary->buf;
        while (!tokenizer_io_buffer_reach_limit_p(primary)) {
            if (dp_isspace(buf[k])) {
                k++;
                size = index;
            } else if (tokenizer_char_single_comment_p(buf + k)) {
                k = tokenizer_aim_skip_single_comment(aim, k);
            } else if (tokenizer_char_multiple_comment_head_p(buf + k)) {
                k = tokenizer_aim_skip_multiple_comment(aim, k);
            } else if (index < READ_BUF_SIZE) {
                secondary->buf[index++] = buf[k++];
            } else {
                assert_exit(READ_BUF_SIZE == index);
                secondary->buf[index] = NULL_CHAR;
                secondary->size = size;

                TOKENIZER_IO_BUFFER_PRINT(aim->secondary);
                return true;
            }

            primary->index = k;
        }
    }

    if (0 == index) {
        return false;
    } else {
        assert_exit(READ_BUF_SIZE >= index);
        secondary->buf[index] = NULL_CHAR;
        secondary->size = size;

        TOKENIZER_IO_BUFFER_PRINT(aim->secondary);
        return true;
    }
}

static inline bool
tokenizer_io_buffer_reach_limit_p(s_io_buffer_t *buffer)
{
    assert_exit(tokenizer_io_buffer_structure_legal_p(buffer));

    if (READ_BUF_SIZE == buffer->index
        || NULL_CHAR == buffer->buf[buffer->index]) {
        return true;
    } else {
        return false;
    }
}

static inline bool
tokenizer_aim_fill_primary_buffer_p(s_tokenizer_aim_t *aim)
{
    uint32 len;
    uint32 rest;
    uint32 offset;

    assert_exit(tokenizer_aim_structure_legal_p(aim));

    if (!tokenizer_io_buffer_reach_limit_p(aim->primary)) {
        return true;
    } else if (dp_feof(aim->fd)) {
        return false;
    } else {
        aim->primary->index = offset = 0;
        rest = READ_BUF_SIZE;

        while (rest != 0 && !dp_feof(aim->fd)) {
            len = dp_fread(aim->primary->buf + offset, 1, rest, aim->fd);
            assert_exit(len <= rest);
            offset += len;
            rest -= len;
        }

        aim->primary->buf[offset] = NULL_CHAR;
        return true;
    }
}

