#ifndef HAVE_DEFINED_REGULAR_H
#define HAVE_DEFINED_REGULAR_H

#define RE_RECOVER_MIN         8u
#define RE_RANGE_START         0u
#define RE_RANGE_LAST          2u
#define RE_RANGE_SIZE          (RE_RANGE_LAST + 1 - RE_RANGE_START)

typedef enum regular_wildcard         e_regular_wildcard_t;
typedef struct regular_recover_buffer s_regular_recover_buffer_t;


struct regular_recover_buffer {
    char *buf;
    uint32 size;  /* bytes count */
    uint32 index;
};

/*
 * Use char ` as the translate prefix for meta char
 * Such as `* match *
 */
enum regular_wildcard {
    RE_WILD_BKT_L  = '(', // left bracket
    RE_WILD_BKT_R  = ')', // right bracket
    RE_WILD_AND    = '&',
    RE_WILD_OR     = '|',
    RE_WILD_STAR   = '*',
    RE_WILD_PLUS   = '+',
    RE_WILD_QUST   = '?',
    RE_WILD_MBKT_L = '[',
    RE_WILD_MBKT_R = ']',
    RE_WILD_TRANS  = '`',
};

enum regular_data_symbol {
    RE_DT_SML_B_SLASH_CHAR = 0x5c, // '\',
    RE_DT_SML_COLON_CHAR   = ':',
    RE_DT_SML_COMMA_CHAR   = ',',
    RE_DT_SML_D_QUOTE_CHAR = 0x22,  // '"'
    RE_DT_SML_DOT_CHAR     = '.',
    RE_DT_SML_EQ_CHAR      = '=',
    RE_DT_SML_GT_CHAR      = '>',
    RE_DT_SML_LT_CHAR      = '<',
    RE_DT_SML_L_BRACE_L    = '{',
    RE_DT_SML_L_BRACE_R    = '}',
    RE_DT_SML_NOT_CHAR     = '!',
    RE_DT_SML_PERCENT_CHAR = '%',
    RE_DT_SML_POUND_CHAR   = '#',
    RE_DT_SML_SEMI_CHAR    = ';',
    RE_DT_SML_SLASH_CHAR   = '/',
    RE_DT_SML_SPACE_CHAR   = ' ',
    RE_DT_SML_SUB_CHAR     = '-',
    RE_DT_SML_TILDE_CHAR   = '~',
    RE_DT_SML_UDRLINE_CHAR = '_',
    RE_DT_SML_XOR_CHAR     = '^',
};

#endif

