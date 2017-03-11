#ifndef HAVE_DEFINED_GRAMMAR_DEFINE_H
#define HAVE_DEFINED_GRAMMAR_DEFINE_H

#if defined DEBUG
    #define GR_LANGUAGE_PRINT(g)           gr_language_print(g)
    #define GR_LANGUAGE_UNFOLD_PRINT(g)    gr_language_after_unfold_print(g)
    #define GR_NULL_PDT_HELPER_PRINT(g)    gr_null_pdt_helper_print(g)
    #define GR_LANGUAGE_ELIMINATE_PRINT(g) gr_language_eliminate_print(g)
#else
    #define GR_LANGUAGE_PRINT(g)
    #define GR_LANGUAGE_UNFOLD_PRINT(g)
    #define GR_NULL_PDT_HELPER_PRINT(g)
    #define GR_LANGUAGE_ELIMINATE_PRINT(g)
#endif

#endif

