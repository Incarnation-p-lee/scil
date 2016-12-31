/*
 * *HACK* Included files order has dependency on types or macros.
 */
#include "depends.h"
#include "finite_automata.h"
#include "nfa.h"
#include "regular.h"
#include "tokenizer.h"

#include "log_external.h"
#include "nfa_external.h"
#include "common_external.h"

#include "token.h"
#include "token_data.h"
#include "token_define.h"
#include "token_language.h"
#include "token_declaration.h"

#if defined DEBUG
    #include "token_debug.c"
#endif

#include "token_char.c"
#include "token_list.c"
#include "token_common.c"
#include "token_language_c.c"

