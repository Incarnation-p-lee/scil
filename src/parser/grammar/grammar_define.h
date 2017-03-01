#ifndef HAVE_DEFINED_GRAMMAR_DEFINE_H
#define HAVE_DEFINED_GRAMMAR_DEFINE_H

#if defined DEBUG
    #define GR_LANGUAGE_PRINT(g) gr_language_print(g)
    #define GR_NULL_PDT_PRINT(g) gr_null_pdt_set_print(g)
#else
    #define GR_LANGUAGE_PRINT(g)
    #define GR_NULL_PDT_PRINT(g)
#endif

#endif

