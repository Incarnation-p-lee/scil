// Automaticaly generated by script/generate_declaration.pl.

#ifndef TOKENIZER_DECLARATION_H
#define TOKENIZER_DECLARATION_H


s_tokenizer_file_list_t * tokenizer_file_list_process(char **file_list, uint32 count);
sint32 main(sint32 argc, char **argv);
static inline bool io_buffer_structure_legal_p(s_io_buffer_t *buf);
static inline bool tokenizer_char_double_quote_p(char *buf);
static inline bool tokenizer_char_multiple_comment_head_p(char *buf);
static inline bool tokenizer_char_multiple_comment_tail_p(char *buf);
static inline bool tokenizer_char_single_comment_p(char *buf);
static inline bool tokenizer_file_list_structure_legal_p(s_tokenizer_file_list_t *tkz_file_list);
static inline bool tokenizer_io_buffer_fill_buffer_p(s_tokenizer_io_buffer_t *aim);
static inline bool tokenizer_io_buffer_fill_primary_buffer_p(s_tokenizer_io_buffer_t *aim);
static inline bool tokenizer_io_buffer_fill_secondary_buffer_final_p(s_tokenizer_io_buffer_t *aim, uint32 index);
static inline bool tokenizer_io_buffer_fill_secondary_buffer_p(s_tokenizer_io_buffer_t *aim);
static inline bool tokenizer_io_buffer_reach_limit_p(s_io_buffer_t *buffer);
static inline bool tokenizer_io_buffer_structure_legal_p(s_tokenizer_io_buffer_t *aim);
static inline bool tokenizer_language_structure_legal_p(s_tokenizer_language_t *lang);
static inline bool tokenizer_language_type_legal_p(e_tokenizer_language_type_t language_type);
static inline bool tokenizer_language_type_p(e_tokenizer_language_type_t type);
static inline s_io_buffer_t * tokenizer_io_buffer_open_buffer_create(void);
static inline s_token_t * tokenizer_file_token_process(s_tokenizer_file_list_t *tkz_file_node);
static inline s_tokenizer_file_list_t * tokenizer_file_list_create(char *fname);
static inline s_tokenizer_file_list_t * tokenizer_file_list_next(s_tokenizer_file_list_t *tkz_file_list);
static inline s_tokenizer_io_buffer_t * tokenizer_io_buffer_open(char *fname);
static inline s_tokenizer_language_t * tokenizer_language_create(char *filename);
static inline uint32 token_lang_punctuation_match(s_tokenizer_language_t *tkz_language, s_token_t *token_head, char *buf);
static inline uint32 tokenizer_io_buffer_skip_multiple_comment(s_tokenizer_io_buffer_t *aim, uint32 index);
static inline uint32 tokenizer_io_buffer_skip_single_comment(s_tokenizer_io_buffer_t *aim, uint32 index);
static inline uint32 tokenizer_io_secondary_buffer_resume(s_io_buffer_t *secondary);
static inline uint32 tokenizer_lang_constant_match(s_tokenizer_language_t *tkz_language, s_token_t *token_head, char *buf);
static inline uint32 tokenizer_lang_identifer_match(s_tokenizer_language_t *tkz_language, s_token_t *token_head, char *buf);
static inline uint32 tokenizer_language_operator_match(s_tokenizer_language_t *tkz_language, s_token_t *token_head, char *buf);
static inline void tokenizer_file_io_buffer_process(s_io_buffer_t *io_buffer, s_tokenizer_language_t *tkz_language, s_token_t *token_head);
static inline void tokenizer_file_list_insert_before(s_tokenizer_file_list_t *tkz_file_list, s_tokenizer_file_list_t *tkz_file_node);
static inline void tokenizer_file_list_node_destroy(s_tokenizer_file_list_t *tkz_file_node);
static inline void tokenizer_file_list_remove(s_tokenizer_file_list_t *tkz_file_list);
static inline void tokenizer_io_buffer_close(s_tokenizer_io_buffer_t *aim);
static inline void tokenizer_language_c_destroy(s_tokenizer_language_t *lang);
static inline void tokenizer_language_c_init(s_tokenizer_language_t *lang);
static inline void tokenizer_language_c_keyword_trie_destroy(s_tokenizer_language_t *lang);
static inline void tokenizer_language_c_keyword_trie_init(s_tokenizer_language_t *lang);
static inline void tokenizer_language_c_nfa_engine_destroy(s_tokenizer_language_t *lang);
static inline void tokenizer_language_c_nfa_engine_init(s_tokenizer_language_t *lang);
static inline void tokenizer_language_destroy(s_tokenizer_language_t *lang);
static inline void tokenizer_language_init(s_tokenizer_language_t *lang);
static void tokenizer_io_buffer_close_print(char *fname);
static void tokenizer_io_buffer_open_print(char *fname);
static void tokenizer_io_buffer_print(s_io_buffer_t *buffer);
void tokenizer_file_list_destroy(s_tokenizer_file_list_t *tkz_file_list);

#endif
