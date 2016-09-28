s_tokenizer_file_list_t *
tokenizer_file_list_process(char **file_list, uint32 count)
{
    char **fname;
    s_tokenizer_file_list_t *tkz_head, *tkz_node;

    if (!file_list || !count) {
        return PTR_INVALID;
    } else {
        tokenizer_file_list_head = dp_malloc(sizeof(*tokenizer_file_list_head));
        tokenizer_file_list_head->filename = tokenizer_file_list_head->token_head = NULL;
        doubly_linked_list_initial(&tokenizer_file_list_head->list);

        fname = file_list;
        while (fname < file_list + count) {
            tokenizer_file_list = tokenizer_file_process(*fname);
            tokenizer_file_list_insert_before(&fake_head, tokenizer_file_list);
            fname++;
        }

        return tokenizer_file_list_head;
    }
}

void
tokenizer_file_list_destroy(s_tokenizer_file_list_t *tokenizer_file_list)
{
    s_tokenizer_file_list_t *tmp;
    s_tokenizer_file_list_t *tokenizer_file_list_next;

    if (tokenizer_file_list_structure_legal_p(tokenizer_file_list)) {
        tmp = tokenizer_file_list;
        do {
            next = tokenizer_file_list_next(tmp);
            tokenizer_file_list_node_destroy(tmp);
            tmp = next;
        } while (tmp != tokenizer_file_list);
    }
}

static inline void
tokenizer_file_list_node_destroy(s_tokenizer_file_list_t *tokenizer_file_list)
{
    assert_exit(tokenizer_file_list_structure_legal_p(tokenizer_file_list));

    dp_free(tokenizer_file_list->filename);
    dp_free(tokenizer_file_list->token_head);
    dp_free(tokenizer_file_list);
}

static inline bool
tokenizer_file_list_structure_legal_p(s_tokenizer_file_list_t *tokenizer_file_list)
{
    if (!tokenizer_file_list) {
        return false;
    } else if (!tokenizer_file_list->filename) {
        return false;
    } else if (!tokenizer_file_list->token_head) {
        return false;
    } else {
        return true;
    }
}

static inline void
tokenizer_file_list_insert_before(s_tokenizer_file_list_t *tokenizer_file_list, s_tokenizer_file_list_t *token_node)
{
    assert_exit(tokenizer_file_list_structure_legal_p(tokenizer_file_list));
    assert_exit(tokenizer_file_list_structure_legal_p(token_node));

    doubly_linked_list_insert_before(&tokenizer_file_list->list, &token_node->list);
}

static inline s_tokenizer_file_list_t *
tokenizer_file_list_next(s_tokenizer_file_list_t *tokenizer_file_list)
{
    assert_exit(tokenizer_file_list_structure_legal_p(tokenizer_file_list));

    return CONTAINS_OF(&tokenizer_file_list->list.next, s_tokenizer_file_list_t, list);
}

static inline s_tokenizer_file_list_t *
tokenizer_file_process(char *fname)
{
    uint32_t len;
    s_tokenizer_file_list_t *tokenizer_file_list;

    assert_exit(filename);

    tokenizer_file_list = dp_malloc(sizeof(*tokenizer_file_list));

    len = dp_strlen(fname) + 1;
    tokenizer_file_list->filename = dp_malloc(len);
    dp_strcpy(tokenizer_file_list->filename, fname);

    tokenizer_file_list->token_head = tokenizer_file_list_process_i(fname);
    return tokenizer_file_list;
}

static inline s_token_t *
tokenizer_file_list_process_i(char *filename)
{
    s_token_t *token_head;
    s_tokenizer_aim_t *aim;
    s_tokenizer_language_t *lang;

    assert_exit(filename);

    lang = tokenizer_lang_create(filename);
    aim = tokenizer_aim_open(filename);

    token_head = dp_malloc(sizeof(s_token_t));
    token_head->type = TK_LEX_HEAD;

    while (tokenizer_aim_fill_secondary_buffer_p(aim)) {
        tokenizer_file_list_process_io_buffer(aim->secondary, token_head, lang);
    }

    tokenizer_aim_close(aim);
    tokenizer_lang_destroy(lang);

    return token_head;
}

static inline void
tokenizer_file_list_process_io_buffer(s_io_buffer_t *buffer, s_token_t *token_head,
    s_tokenizer_language_t *lang)
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
token_lang_operator_match(s_tokenizer_language_t *lang, s_token_t *token_head, char *buf)
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
token_lang_identifer_match(s_tokenizer_language_t *lang, s_token_t *token_head, char *buf)
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
token_lang_constant_match(s_tokenizer_language_t *lang, s_token_t *token_head, char *buf)
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
token_lang_punctuation_match(s_tokenizer_language_t *lang, s_token_t *token_head, char *buf)
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

