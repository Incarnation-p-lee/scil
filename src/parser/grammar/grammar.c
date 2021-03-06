#include "depends.h"
#include "scil_config.h"

#include "common_external.h"
#include "log_external.h"

#include "grammar.h"
#include "grammar_define.h"
#include "grammar_production.h"
#include "grammar_declaration.h"
#include "grammar_string.h"

#if defined DEBUG
    #include "grammar_debug.c"
#endif

#include "grammar_main.c"
#include "grammar_util.c"
#include "grammar_string.c"
#include "grammar_symbol.c"
#include "grammar_production.c"
#include "grammar_language.c"
#include "grammar_ll1.c"
#include "grammar_transform.c"

