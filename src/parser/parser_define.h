#ifndef HAVE_DEFINED_PARSER_DEFINE_H
#define HAVE_DEFINED_PARSER_DEFINE_H

#if defined DEBUG
    #define PARSER_LOGFILE_OPEN(n, m)      log_file_create(n, m)
    #define PARSER_LOGFILE_CLOSE           log_file_close()
#else
    #define PARSER_LOGFILE_OPEN(n, m)
    #define PARSER_LOGFILE_CLOSE
#endif

#endif

