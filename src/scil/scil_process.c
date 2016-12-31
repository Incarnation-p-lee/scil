static inline s_tk_t *
scil_tkz_process(char *filename)
{
    s_tk_t *tk_head;
    s_tkz_file_t *tkz_file;

    assert_exit(filename);

    tkz_file = tkz_file_create(filename);
    tkz_file_tk_process(tkz_file);

    tk_head = tkz_file->tk_list;
    tkz_file->tk_list = NULL;
    tkz_file_destroy(tkz_file);

    return tk_head;
}

static inline void
scil_process_i(char *filename)
{
    s_tk_t *tk_head;

    assert_exit(filename);

    tk_head = scil_tkz_process(filename);
    parser_analysis(tk_head);

    tk_list_destroy(tk_head);
}

static inline void
scil_argument_option_process(char *option)
{
    assert_exit(option);

    if (!dp_strcmp(option, "-t") || !dp_strcmp(option, "--test")) {
        test_scil();
    } else {
        assert_exit(false);
    }
}

static inline void
scil_argument_process(uint32 argc, char **argv)
{
    char **argv_i, *arg;

    assert_exit(argv);

    if (argc == 0) {
        test_scil();
    } else {
        argv_i = argv;

        while (argv_i < argv + argc) {
            arg = *argv_i;

            if (*arg == OPTION_CHAR) {
                scil_argument_option_process(arg);
            } else {
                scil_process_i(arg);
            }

            argv_i++;
        }

        tkz_lang_cache_cleanup();
    }
}

