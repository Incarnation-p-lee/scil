/*
 * *HACK* Included files order has dependency on types or macros.
 */
#include "depends.h"
#include "finite_automata.h"
#include "nfa.h"
#include "token.h"
#include "tokenizer.h"

#include "token_external.h"
#include "nfa_external.h"
#include "test_external.h"

#include "tokenizer_define.h"
#include "tokenizer_print.h"
#include "tokenizer_language.h"
#include "tokenizer_declaration.h"

#if defined DEBUG
    #include "tokenizer_debug.c"
#endif

#include "tokenizer_arguement.c"
#include "tokenizer_print.c"
#include "tokenizer_io_buffer.c"
#include "tokenizer_io_block.c"
#include "tokenizer_language.c"
#include "tokenizer_file.c"
#include "tokenizer_main.c"

