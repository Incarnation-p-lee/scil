/*
 * *HACK* Included files order has dependency on types or macros.
 */
#include "depends.h"
#include "finite_automata.h"
#include "tokenizer.h"

#include "tokenizer_print.h"
#include "tokenizer_declaration.h"

#if defined DEBUG
    #include "tokenizer_debug.c"
#endif

#include "tokenizer_aim.c"
#include "tokenizer_language.c"
#include "tokenizer_main.c"

