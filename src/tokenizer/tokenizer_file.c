s_token_t *
tokenizer_file_process(char *filename)
{
    if (!filename) {
        scil_log_print_and_exit("Attempt to access NULL pointer.\n");
        return PTR_INVALID;
    } else {
        s_token_t * tk = tokenizer_file_process_i(filename);
        dp_free(tk);
        return NULL;
    }
}

static inline s_token_t *
tokenizer_file_process_i(char *filename)
{
    s_token_t *token_head;
    s_tokenizer_aim_t *aim;
    s_token_lang_t *lang;

    assert_exit(filename);

    lang = tokenizer_lang_create(TK_LANG_C);
    aim = tokenizer_aim_open(filename);

    token_head = dp_malloc(sizeof(s_token_t));
    token_head->type = LEX_HEAD;

    while (tokenizer_aim_fill_secondary_buffer_p(aim)) {
        tokenizer_file_process_io_buffer(aim->secondary, token_head, lang);
    }

    tokenizer_aim_close(aim);
    tokenizer_lang_destroy(lang);

    return token_head;
}

static inline void
tokenizer_file_process_io_buffer(s_io_buffer_t *buffer, s_token_t *token_head,
    s_token_lang_t *lang)
{
    char *c;
    uint32 limit, n;
    s_token_t *token;

    assert_exit(token_head);
    assert_exit(tokenizer_lang_structure_legal_p(lang));
    assert_exit(tokenizer_io_buffer_structure_legal_p(buffer));

    i = 0;
    c = buffer->buf;
    limit = buffer->size;

    while (c < limit + buffer->buf) {
        n = nfa_engine_token_match(lang->operator, c, SENTINEL_CHAR);
    }
}

