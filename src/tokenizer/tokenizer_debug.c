static inline void
tkz_file_open_print(char *fname)
{
    assert_exit(fname);

    RETURN_IF_FALSE(log_option_tokenizer_verbose_p());

    log_print("[TKZ] Open source file %s\n", fname);
}

static inline void
tkz_logfile_open(char *binary_name)
{
    uint32 len;
    char *logfile_name, *c;

    assert_exit(binary_name);

    c = dp_strrchr(binary_name, PATH_ISOLATOR);
    c++;

    len = dp_strlen(c);
    logfile_name = dp_malloc(sizeof(char*) * (len + 5));

    dp_memcpy(logfile_name, c, len);
    dp_memcpy(logfile_name + len, TKZ_LOG_SUFFIX, sizeof(TKZ_LOG_SUFFIX));

    log_initial(logfile_name);
    log_config_initial(SCIL_MODULE_TKZ);
    libds_log_file_create();

    dp_free(logfile_name);
}

static inline void
tkz_logfile_close(void)
{
    memory_track_counters_print();
    libds_log_file_close();
    log_close();
}

static inline void
tkz_io_block_print(s_io_block_t *io_block)
{
    assert_exit(tkz_io_block_structure_legal_p(io_block));

    RETURN_IF_FALSE(log_option_tokenizer_verbose_p());

    log_print("\n[TKZ] IO block %d '%s'\n", io_block->index, io_block->buf);
}

static inline void
tkz_lang_c_buffer_print(char *buf)
{
    assert_exit(buf);

    RETURN_IF_FALSE(log_option_tokenizer_verbose_p());

    log_print("[TKZ] matching '%s'\n", buf);
}

static inline bool
tkz_file_structure_legal_p(s_tkz_file_t *tkz_file_list)
{
    if (!tkz_file_list) {
        return false;
    } else if (!tkz_file_list->filename) {
        return false;
    } else {
        return true;
    }
}

static inline bool
tkz_io_block_structure_legal_p(s_io_block_t *io_block)
{
    if (!io_block) {
        return false;
    } else if (!io_block->size) {
        return false;
    } else if (!io_block->buf) {
        return false;
    } else if (io_block->index > io_block->size) {
        return false;
    } else {
        return true;
    }
}

static inline bool
tkz_io_buf_structure_legal_p(s_tkz_io_buffer_t *tkz_io_buf)
{
    if (!tkz_io_buf) {
        return false;
    } else if (!tkz_io_buf->fd) {
        return false;
    } else if (!io_buf_structure_legal_p(tkz_io_buf->io_buffer)) {
        return false;
    } else {
        return true;
    }
}

static inline bool
tkz_lang_type_legal_p(e_tkz_lang_type_t lang_type)
{
    switch (lang_type) {
        case TKZ_LANG_C:
        case TKZ_LANG_CPP:
            return true;
        default:
            return false;
    }
}

static inline bool
io_buf_structure_legal_p(s_io_buffer_t *io_buf)
{
    if (!io_buf) {
        return false;
    } else if (io_buf->index > READ_BUF_SIZE) {
        return false;
    } else {
        return true;
    }
}

static inline bool
tkz_lang_structure_legal_p(s_tkz_lang_t *tkz_lang)
{
    if (!tkz_lang) {
        return false;
    } else if (!tkz_lang->operator || !tkz_lang->identifier
        || !tkz_lang->constant || !tkz_lang->punctuation) {
        return false;
    } else {
        return tkz_lang_type_p(tkz_lang->type);
    }
}

static inline bool
tkz_lang_type_p(e_tkz_lang_type_t tkz_lang_type)
{
    switch (tkz_lang_type) {
        case TKZ_LANG_C:
        case TKZ_LANG_CPP:
            return true;
        default:
            return false;
    }
}

