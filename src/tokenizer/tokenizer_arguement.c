static inline void
tokenizer_arguements_process(uint32 argc, char **argv)
{
    uint32 i;
    char *option;
    char **file_list, **argv_tmp;
    s_tokenizer_file_t *tkz_file_list;

    assert_exit(argv);

    if (0 == argc) {
        test_nfa_engine();
    } else {
        i = 0;
        file_list = dp_malloc(sizeof(void *) * argc);

        argv_tmp = argv;
        while (argv_tmp < argv + argc) {
            option = *argv_tmp;
            if (OPTION_CHAR == option[0]) {
                tokenizer_arguements_option_process(option);
            } else {
                file_list[i++] = option;
            }
            argv_tmp++;
        }

        tkz_file_list = tokenizer_file_process(file_list, i);
        tokenizer_file_print(tkz_file_list);
        dp_free(file_list);
    }
}

static inline void
tokenizer_arguements_option_process(char *option)
{
    assert_exit(option);

    if (!dp_strcmp(option, "-t") || !dp_strcmp(option, "--test")) {
        test_nfa_engine();
    } else {
        assert_exit(false);
    }
}

