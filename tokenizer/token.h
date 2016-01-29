#ifndef HAVE_DEFINED_TOKEN_H
#define HAVE_DEFINED_TOKEN_H

#define BUF_LIMIT 256
#define CHR_NULL  '\0'

enum token_t {
    ID,
    NUM,
    STR,
};

struct token {
    enum token_t type;
    union {
        
        char *str;
    } attr;
};

#endif

