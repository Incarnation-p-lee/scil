// Automaticaly generated by script/generate_external_declaration.pl.

#ifndef HAVE_DEFINED_EXTERNAL_H_common_log_
#define HAVE_DEFINED_EXTERNAL_H_common_log_

extern bool log_option_nfa_verbose_p(void);
extern bool log_option_regular_verbose_p(void);
extern bool log_option_token_verbose_p(void);
extern bool log_option_tokenizer_verbose_p(void);
extern void * memory_track_malloc(uint32 size);
extern void * memory_track_realloc(void *ptr, uint32 size);
extern void assert_print_caution(char *msg, const char *fname, const char *func, uint32 line);
extern void assert_print_exit(char *msg, const char *fname, const char *func, uint32 line);
extern void log_close(void);
extern void log_config_initial(char *module);
extern void log_file_close(void);
extern void log_file_create(char *binary_name, char *module);
extern void log_initial(char *logfile_name);
extern void log_print(const char *format, ...);
extern void log_print_and_exit(const char *format, ...);
extern void memory_track_counters_print(void);
extern void memory_track_free(void *ptr);

#endif

