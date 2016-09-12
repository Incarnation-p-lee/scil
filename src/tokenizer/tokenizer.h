#ifndef HAVE_DEFINED_TOKENIZER_H
#define HAVE_DEFINED_TOKENIZER_H

#define FILENAME_LEN_MAX       256
#define READ_BUF_SIZE          4096
#define READ_ELE_SIZE          64
#define TAIL_BUF_SIZE          128
#define INDEX_INVALID          0xffffffffu
#define BUF_PRINT_LEN          80

// Language C regular expression
#define LANG_C_RE_IDTR         "[A-Za-z_][a-zA-Z0-9_]*"
#define LANG_C_RE_OPTR         "`+|`*|`?|-|/|`||~|^|%|!|&|.|:|=|>|<"
#define LANG_C_RE_CNST         "[0-9]+|\"[a-zA-Z0-9_ #%]+\""
#define LANG_C_RE_PCTT         "`[`],;`(`){}"

typedef struct tokenizer_aim   s_tokenizer_aim_t;
typedef struct io_buffer       s_io_buffer_t;
typedef struct token_lang      s_token_lang_t;

struct tokenizer_aim {
    FILE          *fd;
    s_io_buffer_t *primary;
    s_io_buffer_t *secondary;
    char          fname[FILENAME_LEN_MAX];
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

struct token_lang {
    char                sentinel;
    e_token_lang_type_t type;
    s_nfa_t             *operator;
    s_nfa_t             *identifier;
    s_nfa_t             *constant;
    s_nfa_t             *punctuation;
};

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

