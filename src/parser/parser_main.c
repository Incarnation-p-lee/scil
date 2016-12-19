sint32
main(int argc, char **argv)
{
    PARSER_LOGFILE_OPEN(argv[0], "parser");

    memory_cache_cleanup();
    PARSER_LOGFILE_CLOSE;

    return argc - 1;
}

