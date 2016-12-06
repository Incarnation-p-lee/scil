// Automaticaly generated by script/generate_declaration.pl.

#ifndef TOKENIZER_DECLARATION_H
#define TOKENIZER_DECLARATION_H


sint32 main(int argc, char **argv);
static inline bool io_buf_structure_legal_p(s_io_buffer_t *io_buf);
static inline bool tkz_file_structure_legal_p(s_tkz_file_t *tkz_file_list);
static inline bool tkz_io_block_structure_legal_p(s_io_block_t *io_block);
static inline bool tkz_io_buf_comment_p(s_tkz_io_buffer_t *tkz_io_buf, e_tkz_lang_type_t tkz_type);
static inline bool tkz_io_buf_double_quote_p(s_tkz_io_buffer_t *tkz_io_buf);
static inline bool tkz_io_buf_fill_ip(s_tkz_io_buffer_t *tkz_io_buf);
static inline bool tkz_io_buf_fill_p(s_tkz_io_buffer_t *tkz_io_buf);
static inline bool tkz_io_buf_fill_secondary_tail_p(s_tkz_io_buffer_t *tkz_io_buf);
static inline bool tkz_io_buf_limited_p(s_io_buffer_t *buffer);
static inline bool tkz_io_buf_multiple_comment_head_p(s_tkz_io_buffer_t *tkz_io_buf, e_tkz_lang_type_t tkz_type);
static inline bool tkz_io_buf_multiple_comment_tail_p(s_tkz_io_buffer_t *tkz_io_buf, e_tkz_lang_type_t tkz_type);
static inline bool tkz_io_buf_primary_limited_p(s_tkz_io_buffer_t *tkz_io_buf);
static inline bool tkz_io_buf_secondary_empty_p(s_tkz_io_buffer_t *tkz_io_buf);
static inline bool tkz_io_buf_secondary_fill_p(s_tkz_io_buffer_t *tkz_io_buf, e_tkz_lang_type_t tkz_type);
static inline bool tkz_io_buf_secondary_full_p(s_tkz_io_buffer_t *tkz_io_buf);
static inline bool tkz_io_buf_secondary_loaded_p(s_tkz_io_buffer_t *tkz_io_buf, e_tkz_lang_type_t tkz_type);
static inline bool tkz_io_buf_secondary_overflow_p(s_tkz_io_buffer_t *tkz_io_buf);
static inline bool tkz_io_buf_single_comment_head_p(s_tkz_io_buffer_t *tkz_io_buf, e_tkz_lang_type_t tkz_type);
static inline bool tkz_io_buf_structure_legal_p(s_tkz_io_buffer_t *tkz_io_buf);
static inline bool tkz_lang_structure_legal_p(s_tkz_lang_t *tkz_lang);
static inline bool tkz_lang_type_legal_p(e_tkz_lang_type_t lang_type);
static inline bool tkz_lang_type_p(e_tkz_lang_type_t tkz_lang_type);
static inline char * tkz_io_buf_current(s_tkz_io_buffer_t *tkz_io_buf);
static inline char * tkz_io_buf_secondary_buf(s_tkz_io_buffer_t *tkz_io_buf);
static inline char * tkz_io_buf_secondary_buf_limit(s_tkz_io_buffer_t *tkz_io_buf);
static inline char tkz_io_buf_char_get(s_tkz_io_buffer_t *tkz_io_buf);
static inline char tkz_io_buf_single_comment_end(e_tkz_lang_type_t tkz_type);
static inline e_tkz_lang_type_t tkz_lang_filename_to_type(char *filename);
static inline s_io_block_t * tkz_io_block_create(void);
static inline s_io_buffer_t * io_buf_create(void);
static inline s_tkz_file_t * tkz_file_create(char *fname);
static inline s_tkz_io_buffer_t * tkz_io_buf_create(char *fname);
static inline s_tkz_lang_t * tkz_lang_create(e_tkz_lang_type_t type);
static inline s_tkz_lang_t * tkz_lang_obtain(char *filename);
static inline uint32 tkz_io_block_data_size(char *buf);
static inline uint32 tkz_io_block_fill(s_io_block_t *io_block, char *buf);
static inline uint32 tkz_io_buf_secondary_limit(s_tkz_io_buffer_t *tkz_io_buf);
static inline uint32 tkz_lang_c_tk_match(s_tkz_lang_t *tkz_lang, s_tk_t *tk_head, char *buf);
static inline void io_buffer_print(s_io_buffer_t *buffer);
static inline void tkz_arguements_option_process(char *option);
static inline void tkz_arguements_process(uint32 argc, char **argv);
static inline void tkz_file_destroy(s_tkz_file_t *tkz_file);
static inline void tkz_file_io_buf_process(s_tkz_io_buffer_t *tkz_io_buf, s_tkz_lang_t *tkz_lang, s_tk_t *tk_head);
static inline void tkz_file_open_print(char *fname);
static inline void tkz_file_print(s_tkz_file_t *tkz_file);
static inline void tkz_file_process(char **file_list, uint32 count);
static inline void tkz_file_tk_process(s_tkz_file_t *tkz_file);
static inline void tkz_io_block_destroy(s_io_block_t *io_block);
static inline void tkz_io_block_lang_c_match(s_tkz_lang_t *tkz_lang, s_tk_t *tk_head, s_io_block_t *io_block);
static inline void tkz_io_block_print(s_io_block_t *io_block);
static inline void tkz_io_block_process(s_tkz_lang_t *tkz_lang, s_tk_t *tk_head, s_io_block_t *io_block);
static inline void tkz_io_buf_destroy(s_tkz_io_buffer_t *tkz_io_buf);
static inline void tkz_io_buf_index_advance(s_tkz_io_buffer_t *tkz_io_buf, uint32 count);
static inline void tkz_io_buf_multiple_comment_skip(s_tkz_io_buffer_t *tkz_io_buf, e_tkz_lang_type_t tkz_type);
static inline void tkz_io_buf_secondary_append_with_space(s_tkz_io_buffer_t *tkz_io_buf, bool is_last_space);
static inline void tkz_io_buf_secondary_char_append(s_tkz_io_buffer_t *tkz_io_buf, char c);
static inline void tkz_io_buf_secondary_char_fill(s_tkz_io_buffer_t *tkz_io_buf);
static inline void tkz_io_buf_secondary_resume(s_tkz_io_buffer_t *tkz_io_buf);
static inline void tkz_io_buf_skip_comment(s_tkz_io_buffer_t *tkz_io_buf, e_tkz_lang_type_t tkz_type);
static inline void tkz_io_buf_skip_single_comment(s_tkz_io_buffer_t *tkz_io_buf, e_tkz_lang_type_t tkz_type);
static inline void tkz_lang_c_buffer_print(char *buf);
static inline void tkz_lang_c_destroy(s_tkz_lang_t *tkz_lang);
static inline void tkz_lang_c_init(s_tkz_lang_t *tkz_lang);
static inline void tkz_lang_c_keyword_trie_destroy(s_tkz_lang_t *tkz_lang);
static inline void tkz_lang_c_keyword_trie_init(s_tkz_lang_t *tkz_lang);
static inline void tkz_lang_c_nfa_engine_destroy(s_tkz_lang_t *tkz_lang);
static inline void tkz_lang_c_nfa_engine_init(s_tkz_lang_t *tkz_lang);
static inline void tkz_lang_cache_cleanup(void);
static inline void tkz_lang_destroy(s_tkz_lang_t *tkz_lang);
static inline void tkz_lang_init(s_tkz_lang_t *tkz_lang);
static inline void tkz_logfile_close(void);
static inline void tkz_logfile_open(char *binary_name);

#endif
