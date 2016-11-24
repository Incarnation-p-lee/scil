#ifndef HAVE_DEFINED_TOKENIZER_DEFINE_H
#define HAVE_DEFINED_TOKENIZER_DEFINE_H

#define TKZ_LANGUAGE_SET_SIZE        8

#if defined DEBUG
    #define TKZ_LOGFILE_OPEN(name)   tokenizer_logfile_open(name)
    #define TKZ_LOGFILE_CLOSE        tokenizer_logfile_close()
    #define TKZ_IO_BLOCK_PRINT(io)   tokenizer_io_block_print(io)
#else
    #define TKZ_LOGFILE_OPEN(name)
    #define TKZ_LOGFILE_CLOSE
    #define TKZ_IO_BLOCK_PRINT(io)
#endif

#endif

