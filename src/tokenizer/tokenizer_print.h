#ifndef HAVE_DEFINED_TOKENIZER_PRINT
#define HAVE_DEFINED_TOKENIZER_PRINT

#if defined DEBUG
    #define TOKENIZER_FILE_OPEN_PRINT(x)  tokenizer_file_open_print(x)
    #define TOKENIZER_FILE_CLOSE_PRINT(x) tokenizer_file_close_print(x)
    #define IO_BUFFER_PRINT(x)            io_buffer_print(x)
#else
    #define TOKENIZER_FILE_OPEN_PRINT(x)
    #define TOKENIZER_FILE_CLOSE_PRINT(x)
    #define IO_BUFFER_PRINT(x)
#endif

#endif

