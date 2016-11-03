static void
tokenizer_file_open_print(char *fname)
{
    assert_exit(fname);

    scil_log_print("\n<Open source file %s>\n", fname);
}

static void
io_buffer_print(s_io_buffer_t *buffer)
{
    uint32 i;
    uint32 k;
    char *buf;

    assert_exit(io_buffer_structure_legal_p(buffer));

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
tokenizer_logfile_open(char *binary_name)
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

    scil_log_initial(logfile_name);
    libds_log_file_create();

    dp_free(logfile_name);
}

static inline void
tokenizer_logfile_close(void)
{
    memory_track_counters_print();
    libds_log_file_close();
    scil_log_close();
}

