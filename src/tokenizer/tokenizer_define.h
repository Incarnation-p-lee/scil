#ifndef HAVE_DEFINED_TOKENIZER_DEFINE_H
#define HAVE_DEFINED_TOKENIZER_DEFINE_H

#define TKZ_LANG_SET_SIZE                  8

#if defined DEBUG
    #define TKZ_LOGFILE_OPEN(name)         tkz_logfile_open(name)
    #define TKZ_LOGFILE_CLOSE              tkz_logfile_close()
    #define TKZ_IO_BLOCK_PRINT(io)         tkz_io_block_print(io)
    #define TKZ_LANG_C_BUFFER_PRINT(b)     tkz_lang_c_buffer_print(b)
#else
    #define TKZ_LOGFILE_OPEN(name)
    #define TKZ_LOGFILE_CLOSE
    #define TKZ_IO_BLOCK_PRINT(io)
    #define TKZ_LANG_C_BUFFER_PRINT(b)
#endif

#endif

