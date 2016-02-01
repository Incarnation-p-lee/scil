#ifndef HAVE_DEFINED_TOKEN_H
#define HAVE_DEFINED_TOKEN_H

#define BUF_LIMIT  256
#define CHR_NULL   '\0'
#define NAME_LIMIT 128
#define OPT_LIMIT  4

enum token_t {
    HEAD,
    STE,
    IDT = 0x100,
    NUM,
    STR,
    OPT,
    KWD,
};

struct token {
    enum token_t type;
    union {
        sint32 cst;
        char   name[NAME_LIMIT];
        char   *str;
    };
    struct doubly_linked_list list;
};


static const char *keyword[] = {
    "int", "char", "long", "short", "unsigned", "signed", "float", "double",
    "if", "else", "do", "while", "for", "return", "break", "continue", NULL,
};

#endif

