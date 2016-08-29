#ifndef HAVE_DEFINED_TOKENIZER_H
#define HAVE_DEFINED_TOKENIZER_H

#define FILENAME_LEN_MAX       256
#define READ_BUF_SIZE          4096
#define READ_ELE_SIZE          64
#define TAIL_BUF_SIZE          128
#define INDEX_INVALID          0xffffffffu

typedef enum lexeme          e_lexeme_t;
typedef struct tokenizer_aim s_tokenizer_aim_t;
typedef struct io_buffer     s_io_buffer_t;

enum lexeme {
    LEX_KWRD,  // keyword
    LEX_OPTR,  // operator
    LEX_IDTR,  // identifier
    LEX_CNST,  // constant
    LEX_PCTT,  // punctuation
};

struct tokenizer_aim {
    FILE            *fd;
    s_io_buffer_t   *primary;
    s_io_buffer_t   *secondary;
    char            fname[FILENAME_LEN_MAX];
};

/*
 * Start indicate the read start index of buf
 * If start == READ_BUF_SIZE || NULL_CHAR == buf[start]
 *     All data in buf has been read.
 */
struct io_buffer {
    union {
        uint32 index;  // For primary buf
        uint32 size;   // For secondary buf
    };
    char   buf[READ_BUF_SIZE + 1];
};

#if defined DEBUG
    #define TOKENIZER_AIM_OPEN_PRINT(x)  tokenizer_aim_open_print(x)
    #define TOKENIZER_AIM_CLOSE_PRINT(x) tokenizer_aim_close_print(x)
#else
    #define TOKENIZER_AIM_OPEN_PRINT(x)
    #define TOKENIZER_AIM_CLOSE_PRINT(x)
#endif

#endif

