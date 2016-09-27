#ifndef HAVE_DEFINED_TOKENIZER_PRINT
#define HAVE_DEFINED_TOKENIZER_PRINT

#if defined DEBUG
    #define TOKENIZER_AIM_OPEN_PRINT(x)  tokenizer_aim_open_print(x)
    #define TOKENIZER_AIM_CLOSE_PRINT(x) tokenizer_aim_close_print(x)
    #define TOKENIZER_IO_BUFFER_PRINT(x) tokenizer_io_buffer_print(x)
#else
    #define TOKENIZER_AIM_OPEN_PRINT(x)
    #define TOKENIZER_AIM_CLOSE_PRINT(x)
    #define TOKENIZER_IO_BUFFER_PRINT(x)
#endif

#endif

