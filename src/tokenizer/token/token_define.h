#ifndef HAVE_DEFINED_TOKEN_DEFINE_H
#define HAVE_DEFINED_TOKEN_DEFINE_H

#if defined DEBUG
    #define TK_LANGUAGE_C_PRINT(tk)       token_language_c_log_print(tk)
    #define TOKEN_STRUCTURE_LEGAL_P(tk)   token_structure_legal_p(tk)
    #define TOKEN_STRUCTURE_ILLEGAL_P(tk) token_structure_illegal_p(tk)
#else
    #define TK_LANGUAGE_C_PRINT(tk)
    #define TOKEN_STRUCTURE_LEGAL_P(tk)   NON_NULL_PTR_P(tk)
    #define TOKEN_STRUCTURE_ILLEGAL_P(tk) NULL_PTR_P(tk)
#endif

#define TK_C_KYWD_HOLDER            '-'
#define TK_C_KYWD_ENCODE_SIZE       4


#endif

