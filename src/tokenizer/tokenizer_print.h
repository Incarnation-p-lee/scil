#ifndef HAVE_DEFINED_TOKENIZER_PRINT
#define HAVE_DEFINED_TOKENIZER_PRINT

#if defined DEBUG
    #define TKZ_FILE_OPEN_PRINT(x)  tkz_file_open_print(x)
    #define TKZ_FILE_CLOSE_PRINT(x) tkz_file_close_print(x)
#else
    #define TKZ_FILE_OPEN_PRINT(x)
    #define TKZ_FILE_CLOSE_PRINT(x)
#endif

#endif

