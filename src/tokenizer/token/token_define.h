#ifndef HAVE_DEFINED_TOKEN_DEFINE_H
#define HAVE_DEFINED_TOKEN_DEFINE_H

#if defined DEBUG
    #define TK_LANG_C_PRINT(tk)        tk_lang_c_log_print(tk)
    #define TK_STRUCTURE_LEGAL_P(tk)   tk_structure_legal_p(tk)
    #define TK_STRUCTURE_ILLEGAL_P(tk) tk_structure_illegal_p(tk)
    #define TK_LANG_C_IDTR_PRINT(b)    tk_lang_c_idtr_print(b)
    #define TK_LANG_C_OPTR_PRINT(b)    tk_lang_c_optr_print(b)
    #define TK_LANG_C_CNST_PRINT(b)    tk_lang_c_cnst_print(b)
    #define TK_LANG_C_PCTT_PRINT(b)    tk_lang_c_pctt_print(b)
#else
    #define TK_LANG_C_PRINT(tk)
    #define TK_STRUCTURE_LEGAL_P(tk)   NON_NULL_PTR_P(tk)
    #define TK_STRUCTURE_ILLEGAL_P(tk) NULL_PTR_P(tk)
    #define TK_LANG_C_IDTR_PRINT(b)
    #define TK_LANG_C_OPTR_PRINT(b)
    #define TK_LANG_C_CNST_PRINT(b)
    #define TK_LANG_C_PCTT_PRINT(b)
#endif

#define TK_C_KYWD_HOLDER            '-'
#define TK_C_KYWD_ENCODE_SIZE       4


#endif

