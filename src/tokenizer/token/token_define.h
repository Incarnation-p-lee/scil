#ifndef HAVE_DEFINED_TOKEN_DEFINE_H
#define HAVE_DEFINED_TOKEN_DEFINE_H

#if defined DEBUG
    #define TK_LANGUAGE_C_PRINT(tk)        token_language_c_log_print(tk)
    #define TOKEN_STRUCTURE_LEGAL_P(tk)    token_structure_legal_p(tk)
    #define TOKEN_STRUCTURE_ILLEGAL_P(tk)  token_structure_illegal_p(tk)
    #define TOKEN_LANGUAGE_C_IDTR_PRINT(b) token_language_c_identifier_print(b)
    #define TOKEN_LANGUAGE_C_OPTR_PRINT(b) token_language_c_operator_print(b)
    #define TOKEN_LANGUAGE_C_CNST_PRINT(b) token_language_c_constant_print(b)
    #define TOKEN_LANGUAGE_C_PCTT_PRINT(b) token_language_c_punctuation_print(b)
#else
    #define TK_LANGUAGE_C_PRINT(tk)
    #define TOKEN_STRUCTURE_LEGAL_P(tk)    NON_NULL_PTR_P(tk)
    #define TOKEN_STRUCTURE_ILLEGAL_P(tk)  NULL_PTR_P(tk)
    #define TOKEN_LANGUAGE_C_IDTR_PRINT(b)
    #define TOKEN_LANGUAGE_C_OPTR_PRINT(b)
    #define TOKEN_LANGUAGE_C_CNST_PRINT(b)
    #define TOKEN_LANGUAGE_C_PCTT_PRINT(b)
#endif

#define TK_C_KYWD_HOLDER            '-'
#define TK_C_KYWD_ENCODE_SIZE       4


#endif

