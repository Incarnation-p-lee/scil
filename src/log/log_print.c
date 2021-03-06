void
log_initial(char *logfile_name)
{
    if (logfile_name) {
        log_file = dp_fopen(logfile_name, "w");

        if (!log_file) {
            dp_printf("Failed to create log file %s.\n", logfile_name);
            dp_exit(1);
        }
    }
}

void
log_close(void)
{
    if (log_file) {
        dp_fclose(log_file);
    }
}

void
log_print(const char *format, ...)
{
    dp_va_list vl;

    if (!log_file) {
        dp_printf("[LOG] log file not initialized.\n");
        dp_exit(2);
    }

    if (format) {
        dp_va_start(vl, format);
        dp_vfprintf(log_file, format, vl);
        dp_fflush(log_file);
        dp_va_end(vl);
    }
}

void
log_print_and_exit(const char *format, ...)
{
    dp_va_list vlf, vl;

    if (format) {
        dp_fprintf(log_file, "Err: ");
        dp_va_start(vlf, format);
        dp_va_start(vl, format);

        dp_vfprintf(log_file, format, vlf);
        dp_vprintf(format, vl);

        dp_fflush(log_file);
        dp_va_end(vlf);
        dp_va_end(vl);

        libds_log_file_close();
        log_close();
        memory_cache_cleanup();

        dp_exit(1);
    }
}

