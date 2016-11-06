#ifndef HAVE_DEFINED_TOKENIZER_DEFINE_H
#define HAVE_DEFINED_TOKENIZER_DEFINE_H

#define TKZ_LANGUAGE_SET_SIZE        8

#if defined DEBUG
    #define TKZ_LOGFILE_OPEN(name)   tokenizer_logfile_open(name)
    #define TKZ_LOGFILE_CLOSE        tokenizer_logfile_close()
#else
    #define TKZ_LOGFILE_OPEN(name)
    #define TKZ_LOGFILE_CLOSE
#endif

#endif

