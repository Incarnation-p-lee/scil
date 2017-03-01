// Automaticaly generated by script/generate_declaration.pl.

#ifndef GRAMMAR_DECLARATION_H
#define GRAMMAR_DECLARATION_H


static inline bool gr_language_structure_legal_p(s_gr_lang_t *gr_lang);
static inline bool gr_non_terminal_structure_illegal_p(s_gr_non_tr_t *gr_non_tr);
static inline bool gr_non_terminal_structure_legal_p(s_gr_non_tr_t *gr_non_tr);
static inline bool gr_pdt_body_list_structure_illegal_p(s_gr_body_list_t *gr_body_list);
static inline bool gr_pdt_body_list_structure_legal_p(s_gr_body_list_t *gr_body_list);
static inline bool gr_pdt_body_null_p(s_gr_body_t *gr_body);
static inline bool gr_pdt_body_structure_legal_p(s_gr_body_t *gr_body);
static inline bool gr_pdt_null_contains_p(s_gr_pdt_t *gr_pdt);
static inline bool gr_pdt_null_symbol_set_structure_legal_p(s_gr_null_symbol_set_t *gr_null_set);
static inline bool gr_pdt_structure_legal_p(s_gr_pdt_t *gr_pdt);
static inline bool gr_pdt_symbol_null_p(s_gr_symbol_t *gr_symbol);
static inline bool gr_pdt_symbol_structure_legal_p(s_gr_symbol_t *gr_symbol);
static inline bool gr_pdt_symbol_terminal_p(s_gr_symbol_t *gr_symbol);
static inline bool gr_transform_null_body_inferred_p(s_gr_null_symbol_set_t *gr_null_set, s_gr_body_t *gr_body);
static inline bool gr_transform_null_pdt_inferred_p(s_gr_null_symbol_set_t *gr_null_set, s_gr_pdt_t *gr_pdt);
static inline char * gr_string_head_skip(char *pdt);
static inline e_gr_non_tr_type_t gr_pdt_head_type(s_gr_pdt_t *gr_pdt);
static inline e_gr_non_tr_type_t gr_pdt_symbol_non_terminal_type(s_gr_symbol_t *gr_symbol);
static inline e_gr_non_tr_type_t gr_string_non_terminal_obtain(char *name);
static inline e_gr_tr_type_t gr_string_terminal_obtain(char *name);
static inline s_gr_body_list_t * gr_pdt_body_list(s_gr_pdt_t *gr_pdt);
static inline s_gr_body_list_t * gr_pdt_body_list_create(char *body_list);
static inline s_gr_body_t * gr_pdt_body_create(char *body);
static inline s_gr_body_t * gr_pdt_body_list_body(s_gr_body_list_t *gr_body_list, uint32 i);
static inline s_gr_lang_t * gr_language_create(void);
static inline s_gr_non_tr_t * gr_pdt_head(s_gr_pdt_t *gr_pdt);
static inline s_gr_non_tr_t * gr_pdt_head_create(char *pdt);
static inline s_gr_null_symbol_set_t * gr_pdt_null_symbol_set_create(void);
static inline s_gr_pdt_t * gr_pdt_create(char *pdt);
static inline s_gr_symbol_t * gr_pdt_body_symbol(s_gr_body_t *gr_body, uint32 i);
static inline s_gr_symbol_t * gr_pdt_symbol_create(char *symbol);
static inline uint32 gr_language_pdt_limit(s_gr_lang_t *gr_lang);
static inline uint32 gr_pdt_body_list_limit(s_gr_body_list_t *gr_body_list);
static inline uint32 gr_pdt_body_symbol_count(s_gr_body_t *gr_body);
static inline uint32 gr_pdt_body_symbol_limit(s_gr_body_t *gr_body);
static inline uint32 gr_pdt_null_symbol_set_count(s_gr_null_symbol_set_t *gr_null_set);
static inline uint32 gr_string_body_fill(char *body, uint32 size, char *body_list);
static inline uint32 gr_string_symbol_fill(char *symbol, uint32 size, char *body);
static inline void gr_language_destroy(s_gr_lang_t *gr_lang);
static inline void gr_language_initial(s_gr_lang_t *gr_lang);
static inline void gr_language_pdt_append(s_gr_lang_t *gr_lang, s_gr_pdt_t *gr_pdt);
static inline void gr_language_print(s_gr_lang_t *gr_lang);
static inline void gr_ll1_format(s_gr_lang_t *gr_lang);
static inline void gr_ll1_initial(s_gr_lang_t *gr_lang);
static inline void gr_null_pdt_set_print(s_gr_null_symbol_set_t *gr_null_set);
static inline void gr_pdt_body_destroy(s_gr_body_t *body);
static inline void gr_pdt_body_list_destroy(s_gr_body_list_t *body_list);
static inline void gr_pdt_body_list_print(s_gr_body_list_t *gr_body_list);
static inline void gr_pdt_body_print(s_gr_body_t *gr_body);
static inline void gr_pdt_destroy(s_gr_pdt_t *pdt);
static inline void gr_pdt_head_destroy(s_gr_non_tr_t *gr_non_tr);
static inline void gr_pdt_null_symbol_set_add(s_gr_null_symbol_set_t *gr_null_set, s_gr_pdt_t *gr_pdt);
static inline void gr_pdt_null_symbol_set_destroy(s_gr_null_symbol_set_t *gr_null_set);
static inline void gr_pdt_print(s_gr_pdt_t *gr_pdt, uint32 idx);
static inline void gr_pdt_symbol_destroy(s_gr_symbol_t *gr_symbol);
static inline void gr_string_head_obtain(char *symbol, uint32 size, char *pdt);
static inline void gr_transform_null_pdt_eliminate(s_gr_lang_t *gr_lang);
static inline void gr_transform_null_pdt_seek(s_gr_null_symbol_set_t *gr_null_set, s_gr_lang_t *gr_lang);
static inline void gr_transform_null_pdt_seek_init(s_gr_null_symbol_set_t *gr_null_set, s_array_queue_t *pdt_queue, s_gr_lang_t *gr_lang);
void gr_analysis(void);

#endif
