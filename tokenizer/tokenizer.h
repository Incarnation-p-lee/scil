#ifndef HAVE_DEFINED_TOKENIZER_H
#define HAVE_DEFINED_TOKENIZER_H

enum lexeme {
    LEX_KWRD,  // keyword
    LEX_OPTR,  // operator
    LEX_IDTR,  // identifier
    LEX_CNST,  // constant
    LEX_PCTT,  // punctuation
};

typedef enum lexeme e_lexeme_t;

#endif

