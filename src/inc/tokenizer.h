#ifndef HAVE_DEFINED_TOKENIZER_H
#define HAVE_DEFINED_TOKENIZER_H

#include "token.h"

/*
 * Abbreviation Table
 * +--------------------+
 * | tkz  | tokenizer   |
 * +------+-------------+
 * | tk   | token       |
 * +------+-------------+
 * | lang | language    |
 * +------+-------------+
 * | optr | operator    |
 * +------+-------------+
 * | idtr | identifier  |
 * +------+-------------+
 * | cnst | constant    |
 * +------+-------------+
 * | pctt | punctuation |
 * +------+-------------+
 * | kywd | keyword     |
 * +------+-------------+
 * | buf  | buffer      |
 * +------+-------------+
 */

#define SCIL_MODULE_TKZ              "tkz"

#define FILENAME_LEN_MAX             256
#define READ_BUF_BASE_SIZE           4096
#define READ_BUF_EXTRA_SIZE          4
#define READ_BUF_SIZE                READ_BUF_BASE_SIZE + READ_BUF_EXTRA_SIZE
#define READ_BUF_INDEX_LAST          READ_BUF_BASE_SIZE - 1
#define READ_ELE_SIZE                64
#define TAIL_BUF_SIZE                128
#define INDEX_INVALID                0xffffffffu
#define BUF_PRINT_LEN                128
#define TKZ_LANG_C_COMMENT           '/'
#define OPTION_CHAR                  '-'
#define TKZ_IO_BLOCK_SIZE            256
#define TKZ_LINE_TRACKER_SIZE        512

typedef struct tokenizer_io_buffer   s_tkz_io_buffer_t;
typedef struct io_buffer             s_io_buffer_t;
typedef struct io_block              s_io_block_t;
typedef struct tokenizer_language    s_tkz_lang_t;
typedef struct tokenizer_file        s_tkz_file_t;
typedef enum tokenizer_language_type e_tkz_lang_type_t;

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
 *              |              |- line_nmbr
 *              |              |- io_buffer
 *              |                         |- index
 *              |                         |- size
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
enum tokenizer_language_type {
    TKZ_LANG_C,
    TKZ_LANG_CPP,
    TKZ_LANG_UNSUPPORTED,
};

struct tokenizer_io_buffer {
    FILE                 *fd;
    uint32               line_nmbr;
    s_io_buffer_t        *io_buffer;
};

struct tokenizer_language {
    e_tkz_lang_type_t type;
    s_trie_tree_t     *keyword_trie;
    s_nfa_t           *operator;
    s_nfa_t           *identifier;
    s_nfa_t           *constant;
    s_nfa_t           *punctuation;
};

struct tokenizer_file {
    char              *filename;
    s_tk_t            *tk_list;
    s_tkz_io_buffer_t *tkz_io_buffer;
    s_tkz_lang_t      *tkz_language;
};

struct io_buffer {
    uint32 size;   /* size of legal data in bytes count */
    uint32 index;  /* next index for access */
    char   buf[READ_BUF_SIZE];
};

struct io_block {
    char   *buf;
    uint32 size;
    uint32 index;
    uint32 line_nmbr;
};

#endif

