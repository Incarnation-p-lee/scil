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

    scil_log_print("\n<Close source file %s>\n", fname);
}

