// Automaticaly generated by script/generate_declaration.pl.

#ifndef REGULAR_DECLARATION_H
#define REGULAR_DECLARATION_H


bool regular_char_meta_p(char c);
bool regular_char_translated_p(char c);
bool regular_data_p(char c);
bool regular_opt_binary_p(char c);
bool regular_opt_bracket_left_p(char c);
bool regular_opt_bracket_p(char c);
bool regular_opt_bracket_right_p(char c);
bool regular_opt_p(char c);
bool regular_opt_unary_p(char c);
char * regular_convert_to_reverse_polish(char *re);
char regular_char_translate_resume(char c);
static inline bool regular_opt_and_needed_p(char last, char c);
static inline bool regular_reverse_polish_legal_p(char *polish);
static inline bool regular_stack_top_p(s_array_stack_t *stack, char expected);
static inline char * regular_convert_to_reverse_polish_final(s_array_stack_t *stack, uint32 size);
static inline char * regular_normalize(char *re);
static inline char * regular_opt_and_insert(char *re);
static inline char * regular_range_expand(char *re);
static inline void regular_convert_to_reverse_polish_opt(s_array_stack_t *stack_data, s_array_stack_t *stack_opt, char *c);
static inline void regular_convert_to_reverse_polish_print(char *regular);
static inline void regular_convert_to_reverse_polish_priority_0(s_array_stack_t *stack, char *c);
static inline void regular_convert_to_reverse_polish_priority_1(s_array_stack_t *stack_opt, s_array_stack_t *stack_data, char *c);
static inline void regular_convert_to_reverse_polish_priority_2(s_array_stack_t *stack_opt, s_array_stack_t *stack_data, char *c);
static inline void regular_convert_to_reverse_polish_priority_3(s_array_stack_t *stack_opt, s_array_stack_t *stack_data, char *c);
static inline void regular_opt_and_insert_print(char *regular);
static inline void regular_range_expand_print(char *regular);

#endif
