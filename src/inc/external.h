// Automaticaly generated by script/generate_external_declaration.pl.

#ifndef HAVE_DEFINED_EXTERNAL_H_common_log_
#define HAVE_DEFINED_EXTERNAL_H_common_log_

extern void * memory_track_malloc(uint32 size);
extern void * memory_track_realloc(void *ptr, uint32 size);
extern void assert_print_caution(char *msg, const char *fname, const char *func, uint32 line);
extern void assert_print_exit(char *msg, const char *fname, const char *func, uint32 line);
extern void memory_track_counters_print(void);
extern void memory_track_free(void *ptr);
extern void scil_log_close(void);
extern void scil_log_initial(void);
extern void scil_log_print(const char *format, ...);
extern void scil_log_print_and_exit(const char *format, ...);

#endif

