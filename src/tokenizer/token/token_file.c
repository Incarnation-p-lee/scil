s_token_file_t *
token_file_list_process(char **file_list, uint32 count)
{
    char **fname;
    s_token_file_t *token_file_head, *token_file;

    if (!file_list || !count) {
        return PTR_INVALID;
    } else {
        token_file_head = dp_malloc(sizeof(*token_file_head));
        token_file_head->filename = token_file_head->token_head = NULL;
        doubly_linked_list_initial(&token_file_head->list);

        fname = file_list;
        while (fname < file_list + count) {
            token_file = token_file_process(*fname);
            token_file_insert_before(&fake_head, token_file);
            fname++;
        }

        return token_file_head;
    }
}

void
token_file_list_destroy(s_token_file_t *token_file)
{
    s_token_file_t *tmp;
    s_token_file_t *token_file_next;

    if (token_file_structure_legal_p(token_file)) {
        tmp = token_file;
        do {
            next = token_file_next(tmp);
            token_file_node_destroy(tmp);
            tmp = next;
        } while (tmp != token_file);
    }
}

static inline void
token_file_node_destroy(s_token_file_t *token_file)
{
    assert_exit(token_file_structure_legal_p(token_file));

    dp_free(token_file->filename);
    dp_free(token_file->token_head);
    dp_free(token_file);
}

static inline bool
token_file_structure_legal_p(s_token_file_t *token_file)
{
    if (!token_file) {
        return false;
    } else if (!token_file->filename) {
        return false;
    } else if (!token_file->token_head) {
        return false;
    } else {
        return true;
    }
}

static inline void
token_file_insert_before(s_token_file_t *token_file, s_token_file_t *token_node)
{
    assert_exit(token_file_structure_legal_p(token_file));
    assert_exit(token_file_structure_legal_p(token_node));

    doubly_linked_list_insert_before(&token_file->list, &token_node->list);
}

static inline s_token_file_t *
token_file_next(s_token_file_t *token_file)
{
    assert_exit(token_file_structure_legal_p(token_file));

    return CONTAINS_OF(&token_file->list.next, s_token_file_t, list);
}

static inline s_token_file_t *
token_file_process(char *fname)
{
    uint32_t len;
    s_token_file_t *token_file;

    assert_exit(filename);

    token_file = dp_malloc(sizeof(*token_file));

    len = dp_strlen(fname) + 1;
    token_file->filename = dp_malloc(len);
    dp_strcpy(token_file->filename, fname);

    token_file->token_head = token_file_process_i(fname);
    return token_file;
}

static inline s_token_t *
token_file_process_i(char *filename)
{
    s_token_t *token_head;
    s_tokenizer_aim_t *aim;
    s_tokenizer_lang_t *lang;

    assert_exit(filename);

    lang = tokenizer_lang_create(filename);
    aim = tokenizer_aim_open(filename);

    token_head = dp_malloc(sizeof(s_token_t));
    token_head->type = TK_LEX_HEAD;

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
        c += token_lang_operator_match(lang->operator, token_head, c);
        c += token_lang_identifer_match(lang->operator, token_head, c);
        c += token_lang_constant_match(lang->constant, token_head, c);
        c += token_lang_punctuation_match(lang->punctuation, token_head, c);
    }
}

static inline uint32
token_lang_operator_match(s_tokenizer_lang_t *lang, s_token_t *token_head, char *buf)
{
    assert_exit(buf);
    assert_exit(token_structure_legal_p(token_head));
    assert_exit(tokenizer_lang_structure_legal_p(lang));

    switch (lang->type) {
        case TK_LANG_C:
            return token_lang_c_operator_match(lang->operator, token_head, buf);
        default:
            assert_exit(false);
            return 0;
    }
}

static inline uint32
token_lang_identifer_match(s_tokenizer_lang_t *lang, s_token_t *token_head, char *buf)
{
    uint32 n;
    s_token_t *token;

    assert_exit(buf);
    assert_exit(tokenizer_lang_structure_legal_p(lang));
    assert_exit(token_structure_legal_p(token_head));

    switch (lang->type) {
        case TK_LANG_C:
            n = token_lang_c_identifier_match(lang->identifier, token_head, buf);
            if (n) {
                token = token_list_previous_node(token_head);
                token_language_c_keyword_seek(lang->keyword_trie, token);
            }

            return n;
        default:
            assert_exit(false);
            return 0;
    }
}

static inline uint32
token_lang_constant_match(s_tokenizer_lang_t *lang, s_token_t *token_head, char *buf)
{
    assert_exit(buf);
    assert_exit(tokenizer_lang_structure_legal_p(lang));
    assert_exit(token_structure_legal_p(token_head));

    switch (lang->type) {
        case TK_LANG_C:
            return token_lang_c_constant_match(lang->constant, token_head, buf);
        default:
            assert_exit(false);
            return 0;
    }
}

static inline uint32
token_lang_punctuation_match(s_tokenizer_lang_t *lang, s_token_t *token_head, char *buf)
{
    assert_exit(buf);
    assert_exit(tokenizer_lang_structure_legal_p(lang));
    assert_exit(token_structure_legal_p(token_head));

    switch (lang->type) {
        case TK_LANG_C:
            return token_lang_c_punctuation_match(lang->operator, token_head, buf);
        default:
            assert_exit(false);
            return 0;
    }
}

