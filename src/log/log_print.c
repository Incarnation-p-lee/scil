void
scil_log_initial(void)
{
    if (!log_file) {
        log_file = dp_fopen(logfile_name, "w");
        if (!log_file) {
            dp_printf("Failed to create log file %s.\n", logfile_name);
            dp_exit(1);
        }
    }
}

void
scil_log_close(void)
{
    if (log_file) {
        dp_fclose(log_file);
    }
}

void
scil_log_print(const char *format, ...)
{
    dp_va_list vl;

    if (format) {
        dp_va_start(vl, format);
        dp_vfprintf(log_file, format, vl);
        dp_fflush(log_file);
        dp_va_end(vl);
    }
}

void
scil_log_print_and_exit(const char *format, ...)
{
    dp_va_list vl;

    if (format) {
        dp_va_start(vl, format);
        dp_vfprintf(log_file, format, vl);
        dp_fflush(log_file);
        dp_va_end(vl);

        libds_log_file_close();
        scil_log_close();
        memory_cache_cleanup();
        dp_exit(1);
    }
}

