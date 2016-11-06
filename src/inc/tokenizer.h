#ifndef HAVE_DEFINED_TOKENIZER_H
#define HAVE_DEFINED_TOKENIZER_H

#include "token.h"

#define FILENAME_LEN_MAX             256
#define READ_BUF_BASE_SIZE           4096
#define READ_BUF_EXTRA_SIZE          4
#define READ_BUF_SIZE                READ_BUF_BASE_SIZE + READ_BUF_EXTRA_SIZE
#define READ_BUF_INDEX_LAST          READ_BUF_BASE_SIZE - 1
#define READ_ELE_SIZE                64
#define TAIL_BUF_SIZE                128
#define INDEX_INVALID                0xffffffffu
#define BUF_PRINT_LEN                80
#define PATH_ISOLATOR                '/'
#define TKZ_LANG_C_COMMENT           '/'
#define OPTION_CHAR                  '-'
#define TKZ_LOG_SUFFIX               ".log"
#define TKZ_IO_BLOCK_SIZE            256

typedef struct tokenizer_io_buffer   s_tokenizer_io_buffer_t;
typedef struct io_buffer             s_io_buffer_t;
typedef struct io_block              s_io_block_t;
typedef struct tokenizer_language    s_tokenizer_language_t;
typedef struct tokenizer_file        s_tokenizer_file_t;
typedef enum tokenizer_language_type e_tokenizer_language_type_t;

enum tokenizer_language_type {
    TKZ_LANG_C,
    TKZ_LANG_CPP,
    TKZ_LANG_UNSUPPORTED,
};

struct tokenizer_io_buffer {
    FILE          *fd;
    s_io_buffer_t *primary;
    s_io_buffer_t *secondary;
};

struct tokenizer_language {
    e_tokenizer_language_type_t type;
    s_trie_tree_t               *keyword_trie;
    s_nfa_t                     *operator;
    s_nfa_t                     *identifier;
    s_nfa_t                     *constant;
    s_nfa_t                     *punctuation;
};

struct tokenizer_file {
    char                    *filename;
    s_token_t               *tk_list;
    s_tokenizer_io_buffer_t *tkz_io_buffer;
    s_tokenizer_language_t  *tkz_language;
};

/*
 * TOKENIZER data structure layout
 * tokenizer_file
 *              |- filename
 *              |- tk_list
 *              |        |- type
 *              |        |- data
 *              |        |- list
 *              |- tkz_io_buffer
 *              |              |- fd
 *              |              |- primary
 *              |              |        |- index/size
 *              |              |        |- buf
 *              |              |- secondary
 *              |                         |- index/size
 *              |                         |- buf
 *              |- tkz_language
 *              |             |- type
 *              |             |- keyword_trie
 *              |             |- operator
 *              |             |- identifier
 *              |             |- constant
 *              |             |- punctuation
 * NOTE:
 * 1. tkz_language points to one static data that holds all supported language
 */

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
    char   buf[READ_BUF_BASE_SIZE + READ_BUF_EXTRA_SIZE];
};

struct io_block {
    uint32 size;
    char   *block_buf;
    char   *iterate_buf;
};

#endif

