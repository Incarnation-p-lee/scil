void
log_initial(void)
{
    if (!log_file) {
        log_file = dp_fopen(log_file_name, "w");
        if (!log_file) {
            dp_printf("Failed to create log file %s.\n", log_file_name);
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

    if (format) {
        dp_va_start(vl, format);
        dp_vfprintf(log_file, format, vl);
        dp_fflush(log_file);
        dp_va_end(vl);
    }
}

