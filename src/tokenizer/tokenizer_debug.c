static void
tokenizer_aim_open_print(char *fname)
{
    assert_exit(fname);

    scil_log_print("\n<Open source file %s>\n", fname);
}

static void
tokenizer_aim_close_print(char *fname)
{
    assert_exit(fname);

    scil_log_print("\n<Close source file %s>\n\n", fname);
}

static void
tokenizer_io_buffer_print(s_io_buffer_t *buffer)
{
    uint32 i;
    uint32 k;
    char *buf;

    assert_exit(tokenizer_io_buffer_structure_legal_p(buffer));

    i = k = 0;
    buf = buffer->buf;
    scil_log_print("<Print IO buffer %d>\n[\n ", buffer->index);

    while (buf[i]) {
        scil_log_print("%c", buf[i]);
        k++;
        i++;
        if (BUF_PRINT_LEN == k) {
            k = 0;
            scil_log_print("\n ");
        }
    }

    if (k) {
        scil_log_print("\n");
    }

    assert_exit(i <= READ_BUF_SIZE);
    scil_log_print("]\n<End of Print IO buffer>\n\n");
}

static inline void
token_lang_c_keyword_trie_legal_p(s_token_lang_c_keyword_t *keyword_trie)
{
    char **tmp;
    s_token_lang_c_keyword_t *root;

    assert_exit(token_lang_c_keyword_structure_legal_p(keyword_trie));

    root = keyword_trie;
    tmp = lang_c_keyword;

    while (tmp < lang_c_keyword + ARRAY_SIZE_OF(lang_c_keyword)) {
        if (TK_C_IDTR_NONE == token_lang_c_keyword_match(root, *tmp)) {
            return false;
        }
        tmp++;
    }

    return true;
}

