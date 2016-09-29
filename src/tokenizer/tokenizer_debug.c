static void
tokenizer_file_open_print(char *fname)
{
    assert_exit(fname);

    scil_log_print("\n<Open source file %s>\n", fname);
}

static void
tokenizer_file_close_print(char *fname)
{
    assert_exit(fname);

    scil_log_print("\n<Close source file %s>\n\n", fname);
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

