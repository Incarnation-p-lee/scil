#ifndef HAVE_DEFINED_GRAMMAR_PRODUCTION_H
#define HAVE_DEFINED_GRAMMAR_PRODUCTION_H

/*
 * PRODUCTION of grammar are expression production of terminal
 * and non-terminal symbol list.
 * Here we define the context-free-grammar symbol name as convention,
 * (no matter LL-1, LR).
 *
 * KNOWN terminal symbol
 *   id   - identifier token
 *   cnst - constant token
 *   kywd - keyword token
 *   pctt - punctuation token
 *   optr - operator token
 *
 * NON-terminal symbol ($ means empty)
 */

/* 
 * '`' means or, we choose this because it cannot appear in c code.
 * Example: func_link -> static`extern
 *          func_link -> static
 *          func_link -> extern
 */
static char *gr_language[] = {
    "func_decl -> func_link func_attr type id ( param ) ;",
    "func_link -> static`extern`$",
    "func_attr -> inline`$",
    "type -> type_name type_ptr",
    "type_ptr -> type_ptr`*`$",
    "type_name -> void`type_set`type_base",
    "type_set -> type_set_head id",
    "type_set_head -> struct`union`enum",
    "type_base -> type_int`type_float",
    "type_int -> type_int_attr type_int_base",
    "type_int_attr -> unsigned`signed`$",
    "type_int_base -> char`int`short`long`long long",
    "type_float -> float`double",
    "param -> param_head param_tail",
    "param_head -> param_head`type id ,`$",
    "param_tail -> type id`void",
#if 0
    "func_impl -> func_link func_attr type id ( param ) func_body",
    "func_body -> { st_list }",
    "st_list -> { st_raw_list }`st_raw_list",
    "st_raw_list -> st_raw_list st`st",
    "st -> st_body ;`$",
    "st_body -> st_var_decl`st_var_def`st_call`st_assign`st_control`st_return",
    "st_var_decl -> type id",
    "st_var_def -> type id = expr",
    "st_call -> id ( call_param )",
    "call_param -> call_param_head call_param_tail",
    "call_param_head -> call_param_head`expr`$",
#endif
};

#endif

