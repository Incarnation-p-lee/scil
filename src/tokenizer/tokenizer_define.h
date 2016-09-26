#ifndef HAVE_DEFINED_TOKENIZER_DEFINE_H
#define HAVE_DEFINED_TOKENIZER_DEFINE_H

#define TOKEN_IDTR_LEN                     256
#define TK_C_OPTR_SZ                       8
#define TK_C_OPTR_DUAL(x, y)               (((x) << TK_C_OPTR_SZ) + (y))
#define TK_C_OPTR_TRIPLE(x, y, z)          ((TK_C_OPTR_DUAL(x, y) << TK_C_OPTR_SZ) + (z))
#define TK_LANG_DUAL(x, y)                 TK_C_OPTR_DUAL(x, y)

#endif

