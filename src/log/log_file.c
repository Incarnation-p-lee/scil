void
log_file_create(char *binary_name)
{
    uint32 len;
    char *logfile_name, *c;

    if (binary_name) {
        c = dp_strrchr(binary_name, PATH_ISOLATOR);
        c++;

        len = dp_strlen(c);
        logfile_name = dp_malloc(sizeof(char*) * (len + 5));

        dp_memcpy(logfile_name, c, len);
        dp_memcpy(logfile_name + len, SCIL_LOG_SUFFIX, sizeof(SCIL_LOG_SUFFIX));

        log_initial(logfile_name);
        libds_log_file_create();

        dp_free(logfile_name);
    }
}

void
log_file_close(void)
{
    memory_track_counters_print();
    libds_log_file_close();
    log_close();
}

