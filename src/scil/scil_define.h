#ifndef HAVE_DEFINED_MAIN_H
#define HAVE_DEFINED_MAIN_H

#if defined DEBUG
    #define LOG_FILE_CREATE(n) log_file_create(n)
    #define LOG_FILE_CLOSE()   log_file_close()

#else
    #define LOG_FILE_CREATE(n)
    #define LOG_FILE_CLOSE()

#endif

#endif

