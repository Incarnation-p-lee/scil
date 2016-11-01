#include "depends.h"
#include "tokenizer.h"
#include "tokenizer_declaration.h"

sint32
main(int argc, char **argv)
{
    char *logfile;

    logfile = tokenizer_logfile_name_create(argv[0]);
    scil_log_initial(logfile);
    libds_log_file_create();

    /* skip binary name for argv[0] */
    tokenizer_arguements_process(argc - 1, argv + 1);

    tokenizer_logfile_name_destroy(logfile);
    memory_cache_cleanup();
    memory_track_counters_print();

    libds_log_file_close();
    scil_log_close();

    return 0;
}

static inline void
tokenizer_arguements_process(uint32 argc, char **argv)
{
    uint32 i;
    char *option;
    char **file_list, **argv_tmp;

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

        tokenizer_file_list_process(file_list, i);
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

static inline char *
tokenizer_logfile_name_create(char *binary_name)
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

    return logfile_name;
}

static inline void
tokenizer_logfile_name_destroy(char *logfile_name)
{
    assert_exit(logfile_name);

    dp_free(logfile_name);
}

