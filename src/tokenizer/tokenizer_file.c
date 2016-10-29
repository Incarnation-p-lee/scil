s_tokenizer_file_list_t *
tokenizer_file_list_process(char **file_list, uint32 count)
{
    char **fname;
    s_tokenizer_file_list_t tkz_file_fake, *tkz_file_node;

    if (!file_list || !count) {
        return PTR_INVALID;
    } else {
        doubly_linked_list_initial(&tkz_file_fake.list);

        fname = file_list;
        while (fname < file_list + count) {
            tkz_file_node = tokenizer_file_list_create(*fname);
            tokenizer_file_token_process(tkz_file_node);

            tokenizer_file_list_insert_before(&tkz_file_fake, tkz_file_node);
            fname++;
        }

        tkz_file_node = tokenizer_file_list_next(&tkz_file_fake);
        tokenizer_file_list_remove(&tkz_file_fake);

        return tkz_file_node;
    }
}

static inline s_tokenizer_file_list_t *
tokenizer_file_list_create(char *fname)
{
    uint32 len;
    s_tokenizer_file_list_t *tkz_file_node;

    assert_exit(fname);

    tkz_file_node = dp_malloc(sizeof(*tkz_file_node));

    len = dp_strlen(fname) + 1;
    tkz_file_node->filename = dp_malloc(len);
    dp_strcpy(tkz_file_node->filename, fname);

    tkz_file_node->token_list = NULL;
    doubly_linked_list_initial(&tkz_file_node->list);

    return tkz_file_node;
}

void
tokenizer_file_list_destroy(s_tokenizer_file_list_t *tkz_file_list)
{
    s_tokenizer_file_list_t *tkz_file_tmp;
    s_tokenizer_file_list_t *tkz_file_next;

    if (tokenizer_file_list_structure_legal_p(tkz_file_list)) {
        tkz_file_tmp = tkz_file_list;
        do {
            tkz_file_next = tokenizer_file_list_next(tkz_file_tmp);
            tokenizer_file_list_node_destroy(tkz_file_tmp);
            tkz_file_tmp = tkz_file_next;
        } while (tkz_file_tmp != tkz_file_list);
    }
}

static inline void
tokenizer_file_list_node_destroy(s_tokenizer_file_list_t *tkz_file_node)
{
    assert_exit(tokenizer_file_list_structure_legal_p(tkz_file_node));

    token_language_c_destroy(tkz_file_node->token_list);
    dp_free(tkz_file_node->filename);
    dp_free(tkz_file_node);
}

static inline bool
tokenizer_file_list_structure_legal_p(s_tokenizer_file_list_t *tkz_file_list)
{
    if (!tkz_file_list) {
        return false;
    } else if (!tkz_file_list->filename) {
        return false;
    } else {
        return true;
    }
}

static inline void
tokenizer_file_list_insert_before(s_tokenizer_file_list_t *tkz_file_list,
    s_tokenizer_file_list_t *tkz_file_node)
{
    assert_exit(tokenizer_file_list_structure_legal_p(tkz_file_list));
    assert_exit(tokenizer_file_list_structure_legal_p(tkz_file_node));

    doubly_linked_list_insert_before(&tkz_file_list->list, &tkz_file_node->list);
}

static inline void
tokenizer_file_list_remove(s_tokenizer_file_list_t *tkz_file_list)
{
    s_doubly_linked_list_t *list_removed;

    assert_exit(tokenizer_file_list_structure_legal_p(tkz_file_list));

    list_removed = &tkz_file_list->list;
    doubly_linked_list_remove(&list_removed);
}

static inline s_tokenizer_file_list_t *
tokenizer_file_list_next(s_tokenizer_file_list_t *tkz_file_list)
{
    assert_exit(tokenizer_file_list_structure_legal_p(tkz_file_list));

    return CONTAINS_OF(tkz_file_list->list.next, s_tokenizer_file_list_t, list);
}

static inline void
tokenizer_file_token_process(s_tokenizer_file_list_t *tkz_file_node)
{
    s_token_t *token_head;
    s_tokenizer_language_t *tkz_language;
    s_tokenizer_io_buffer_t *tkz_io_buffer;

    assert_exit(tokenizer_file_list_structure_legal_p(tkz_file_node));

    tkz_language = tokenizer_language_create(tkz_file_node->filename);
    tkz_io_buffer = tokenizer_io_buffer_create(tkz_file_node->filename);

    token_head = dp_malloc(sizeof(s_token_t));
    token_head->type = TK_LEX_HEAD;
    doubly_linked_list_initial(&token_head->list);

    while (tokenizer_io_buffer_fill_buffer_p(tkz_io_buffer, tkz_language->type)) {
        tokenizer_file_io_buffer_process(tkz_io_buffer->secondary, tkz_language,
            token_head);
    }

    tokenizer_language_destroy(tkz_language);
    tokenizer_io_buffer_destroy(tkz_io_buffer);

    tkz_file_node->token_list = token_head;
}

static inline void
tokenizer_file_io_buffer_process(s_io_buffer_t *io_buffer,
    s_tokenizer_language_t *tkz_language, s_token_t *token_head)
{
    char *c;
    uint32 limit;
    s_io_block_t *io_block;

    assert_exit(token_head);
    assert_exit(tokenizer_language_structure_legal_p(tkz_language));
    assert_exit(io_buffer_structure_legal_p(io_buffer));

    c = io_buffer->buf;
    limit = io_buffer->size;
    io_block = tokenizer_io_block_create();

    while (c < limit + io_buffer->buf) {
        c += tokenizer_io_block_fill(io_block, c);
        tokenizer_io_block_process(tkz_language, token_head, io_block);
    }

    tokenizer_io_block_destroy(io_block);
}

