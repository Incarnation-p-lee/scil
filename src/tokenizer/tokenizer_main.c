sint32
main(int argc, char **argv)
{
    TKZ_LOGFILE_OPEN(argv[0]);

    // skip binary name for argv[0]
    tokenizer_arguements_process(argc - 1, argv + 1);
    memory_cache_cleanup();

    TKZ_LOGFILE_CLOSE;

    return 0;
}

