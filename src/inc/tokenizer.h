#ifndef HAVE_DEFINED_TOKENIZER_H
#define HAVE_DEFINED_TOKENIZER_H

#include "token.h"

#define FILENAME_LEN_MAX             256
#define READ_BUF_SIZE                4096
#define READ_ELE_SIZE                64
#define TAIL_BUF_SIZE                128
#define INDEX_INVALID                0xffffffffu
#define BUF_PRINT_LEN                80
#define PATH_ISOLATOR                '/'
#define OPTION_CHAR                  '-'
#define TKZ_LOG_SUFFIX               ".log"

typedef struct tokenizer_io_buffer   s_tokenizer_io_buffer_t;
typedef struct io_buffer             s_io_buffer_t;
typedef struct tokenizer_language    s_tokenizer_language_t;
typedef struct tokenizer_file_list   s_tokenizer_file_list_t;
typedef enum tokenizer_language_type e_tokenizer_language_type_t;

enum tokenizer_language_type {
    TKZ_LANG_C   = 'c',
    TKZ_LANG_CPP = TK_C_OPTR_TRIPLE('c', 'p', 'p'),

};

struct tokenizer_io_buffer {
    FILE          *fd;
    s_io_buffer_t *primary;
    s_io_buffer_t *secondary;
};

struct tokenizer_language {
    e_tokenizer_language_type_t type;
    void                       *keyword_trie;
    s_nfa_t                    *operator;
    s_nfa_t                    *identifier;
    s_nfa_t                    *constant;
    s_nfa_t                    *punctuation;
};

struct tokenizer_file_list {
    char                    *filename;
    s_token_t               *token_list;
    s_doubly_linked_list_t  list;
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

#endif

