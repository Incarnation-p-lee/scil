/*
 * *HACK* Included files order has dependency on types or macros.
 */
#include "depends.h"
#include "finite_automata.h"
#include "regular.h"
#include "regular_external.h"

#include "nfa.h"
#include "nfa_data.h"
#include "nfa_print.h"
#include "nfa_define.h"
#include "nfa_declaration.h"

#if defined DEBUG
    #include "nfa_debug.c"
#endif

#include "nfa_common.c"
#include "nfa_closure.c"
#include "nfa_subset_construction.c"
#include "nfa_engine.c"
#include "nfa_simplify.c"

