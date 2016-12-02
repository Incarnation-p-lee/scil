// Automaticaly generated by script/generate_declaration.pl.

#ifndef REGULAR_DECLARATION_H
#define REGULAR_DECLARATION_H


bool regular_char_bracket_left_p(char c);
bool regular_char_bracket_p(char c);
bool regular_char_bracket_right_p(char c);
bool regular_char_data_p(char c);
bool regular_char_translated_p(char c);
bool regular_char_wildcard_binary_p(char c);
bool regular_char_wildcard_p(char c);
bool regular_char_wildcard_unary_p(char c);
char * regular_polish_process(char *re);
char regular_char_translate_resume(char c);
static inline bool regular_char_and_needed_p(char last, char c);
static inline bool regular_char_and_prefix_p(char prefix);
static inline bool regular_char_and_suffix_p(char suffix);
static inline bool regular_char_data_symbol_p(char c);
static inline bool regular_polish_legal_p(char *polish);
static inline bool regular_recover_structure_legal_p(s_regular_recover_t *recover);
static inline bool regular_stack_top_p(s_array_stack_t *stack, char expected);
static inline bool regular_stack_top_wildcard_unary_p(s_array_stack_t *stack);
static inline char * regular_char_and_insert(char *re);
static inline char * regular_normalize(char *re);
static inline char * regular_polish_process_final(s_array_stack_t *stack, uint32 size);
static inline char * regular_polish_process_i(char *normal);
static inline s_regular_recover_t * regular_recover_create(uint32 bytes_count);
static inline uint32 regular_range_unfold(s_regular_recover_t *recover, char *re);
static inline uint32 regular_range_unfold_i(s_regular_recover_t *recover, char *re);
static inline void regular_char_and_insert_print(char *regular);
static inline void regular_convert_to_polish_print(char *regular);
static inline void regular_polish_optr_bracket_eliminate(s_array_stack_t *stack_opt, s_array_stack_t *stack_data);
static inline void regular_polish_optr_p0_process(s_array_stack_t *stack, char *c);
static inline void regular_polish_optr_p1_process(s_array_stack_t *stack_opt, s_array_stack_t *stack_data, char *c);
static inline void regular_polish_optr_p2_process(s_array_stack_t *stack_opt, s_array_stack_t *stack_data, char *c);
static inline void regular_polish_optr_p3_process(s_array_stack_t *stack_opt, s_array_stack_t *stack_data, char *c);
static inline void regular_polish_optr_process(s_array_stack_t *stack_data, s_array_stack_t *stack_opt, char *c);
static inline void regular_range_recover(s_regular_recover_t *recover, char *re);
static inline void regular_range_recover_print(char *regular);
static inline void regular_recover_destroy(s_regular_recover_t *recover);

#endif
