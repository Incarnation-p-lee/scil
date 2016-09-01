s_token_t *
tokenizer_process_file(char *filename)
{
    if (!filename) {
        scil_log_print_and_exit("Attempt to access NULL pointer.\n");
        return NULL;
    } else {
        return tokenizer_process_file_i(filename);
    }
}

static inline s_token_t *
tokenizer_process_file_i(char *filename)
{
    s_token_t *token_head;
    s_tokenizer_aim_t *aim;

    assert_exit(filename);

    token_head = dp_malloc(sizeof(s_token_t));
    aim = tokenizer_aim_open(filename);

    while (tokenizer_aim_fill_secondary_buffer_p(aim)) {
        tokenizer_process_io_buffer(aim->secondary, token_head);
    }

    tokenizer_aim_close(aim);

    return token_head;
}

static inline void
tokenizer_process_io_buffer(s_io_buffer_t *buffer, s_token_t *token)
{
    uint32 i;
    uint32 limit;

    assert_exit(token);
    assert_exit(tokenizer_io_buffer_structure_legal_p(buffer));

    i = 0;
    limit = buffer->size;
    while (i < limit) {
    }
}

