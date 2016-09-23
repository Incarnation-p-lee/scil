#ifndef HAVE_DEFINED_TOKENIZER_H
#define HAVE_DEFINED_TOKENIZER_H

#define FILENAME_LEN_MAX         256
#define READ_BUF_SIZE            4096
#define READ_ELE_SIZE            64
#define TAIL_BUF_SIZE            128
#define INDEX_INVALID            0xffffffffu
#define BUF_PRINT_LEN            80

// Language C regular expression
#define LANG_C_RE_IDTR           "[A-Za-z_][a-zA-Z0-9_]*"
#define LANG_C_RE_OPTR           "`+|`*|`?|-|/|`||~|^|%|!|&|.|:|=|>|<"
#define LANG_C_RE_CNST           "[0-9]+|\"[a-zA-Z0-9_ #%]+\""
#define LANG_C_RE_PCTT           "`[`],;`(`){}"

typedef struct tokenizer_aim     s_tokenizer_aim_t;
typedef struct io_buffer         s_io_buffer_t;
typedef struct tokenizer_lang    s_tokenizer_lang_t;
typedef struct token_file        s_token_file_t;
typedef enum tokenizer_lang_type e_tokenizer_lang_type_t;

enum tokenizer_lang_type {
    TK_LANG_C    = 'c',
    TK_LANG_CPP  = TK_C_OPTR_TRIPLE('c', 'p', 'p'),

};

struct tokenizer_aim {
    FILE          *fd;
    s_io_buffer_t *primary;
    s_io_buffer_t *secondary;
    char          fname[FILENAME_LEN_MAX];
};

struct tokenizer_lang {
    e_tokenizer_lang_type_t type;
    void                    *keyword_trie;
    s_nfa_t                 *operator;
    s_nfa_t                 *identifier;
    s_nfa_t                 *constant;
    s_nfa_t                 *punctuation;
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

struct token_file {
    char                   *filename;
    s_token_t              *token_head;
    s_doubly_linked_list_t list;
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

