int
main(int argc, char **argv)
{
    LOG_FILE_CREATE(argv[0]);

    scil_argument_process(argc - 1, argv + 1);
    memory_cache_cleanup();

    LOG_FILE_CLOSE();

    return 0;
}

