#ifndef HAVE_DEFINED_GRAMMAR_DEFINE_H
#define HAVE_DEFINED_GRAMMAR_DEFINE_H

#if defined DEBUG
    #define GRAMMAR_LANGUAGE_PRINT(g) grammar_language_print(g)
#else
    #define GRAMMAR_LANGUAGE_PRINT(g)
#endif

#endif

