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
    s_tokenizer_lang_t *lang;

    assert_exit(filename);

    lang = tokenizer_lang_create(filename);
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
    s_tokenizer_lang_t *lang)
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
        n = token_lang_match_operator(lang->operator, token_head, c);
        c += n;
        n = token_lang_match_identifer(lang->operator, token_head, c);
    }
}

static inline uint32
token_lang_match_operator(s_tokenizer_lang_t *lang, s_token_t *token_head, char *buf)
{
    assert_exit(buf);
    assert_exit(tokenizer_lang_structure_legal_p(lang));
    assert_exit(token_structure_legal_p(token_head));

    switch (lang->type) {
        case TK_LANG_C:
            return token_lang_c_match_operator(lang->operator, token_head, buf);
        default:
            assert_exit(false);
            return 0;
    }
}

