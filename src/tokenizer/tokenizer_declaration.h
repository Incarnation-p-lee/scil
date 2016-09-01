// Automaticaly generated by script/generate_declaration.pl.

#ifndef TOKENIZER_DECLARATION_H
#define TOKENIZER_DECLARATION_H


s_token_t * tokenizer_process_file(char *filename);
static inline bool tokenizer_aim_fill_buffer_p(s_tokenizer_aim_t *aim);
static inline bool tokenizer_aim_fill_primary_buffer_p(s_tokenizer_aim_t *aim);
static inline bool tokenizer_aim_fill_secondary_buffer_final_p(s_tokenizer_aim_t *aim, uint32 index);
static inline bool tokenizer_aim_fill_secondary_buffer_p(s_tokenizer_aim_t *aim);
static inline bool tokenizer_aim_structure_legal_p(s_tokenizer_aim_t *aim);
static inline bool tokenizer_char_double_quote_p(char *buf);
static inline bool tokenizer_char_multiple_comment_head_p(char *buf);
static inline bool tokenizer_char_multiple_comment_tail_p(char *buf);
static inline bool tokenizer_char_single_comment_p(char *buf);
static inline bool tokenizer_io_buffer_reach_limit_p(s_io_buffer_t *buffer);
static inline bool tokenizer_io_buffer_structure_legal_p(s_io_buffer_t *buf);
static inline s_io_buffer_t * tokenizer_aim_open_buffer_create(void);
static inline s_token_t * tokenizer_process_file_i(char *filename);
static inline s_tokenizer_aim_t * tokenizer_aim_open(char *fname);
static inline uint32 tokenizer_aim_skip_multiple_comment(s_tokenizer_aim_t *aim, uint32 index);
static inline uint32 tokenizer_aim_skip_single_comment(s_tokenizer_aim_t *aim, uint32 index);
static inline uint32 tokenizer_io_secondary_buffer_resume(s_io_buffer_t *secondary);
static inline void tokenizer_aim_close(s_tokenizer_aim_t *aim);
static inline void tokenizer_process_io_buffer(s_io_buffer_t *buffer, s_token_t *token);
static void tokenizer_aim_close_print(char *fname);
static void tokenizer_aim_open_print(char *fname);
static void tokenizer_io_buffer_print(s_io_buffer_t *buffer);

#endif
