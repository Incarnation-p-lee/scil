// Automaticaly generated by script/generate_declaration.pl.

#ifndef COMMON_DECLARATION_H
#define COMMON_DECLARATION_H


bool config_grammar_verbose_p(void);
bool config_nfa_verbose_p(void);
bool config_regular_verbose_p(void);
bool config_token_verbose_p(void);
bool config_tokenizer_verbose_p(void);
char * string_space_skip(char *string);
static inline bool config_option_line_empty_p(char *config_buf);
static inline uint32 config_option_grammar_type(char *assign);
static inline void config_option_bool_set(char *option);
static inline void config_option_line_parse(char *option);
static inline void config_option_parse(FILE *config);
static inline void config_option_type_set(char *option, char *assign);
uint32 config_grammar_type(void);
void * memory_track_malloc(uint32 size);
void * memory_track_realloc(void *ptr, uint32 size);
void assert_print_caution(char *msg, const char *fname, const char *func, uint32 line);
void assert_print_exit(char *msg, const char *fname, const char *func, uint32 line);
void config_initial(void);
void memory_track_counters_print(void);
void memory_track_free(void *ptr);

#endif
