// Automaticaly generated by script/generate_declaration.pl.

#ifndef COMMON_DECLARATION_H
#define COMMON_DECLARATION_H


void * memory_track_malloc(uint32 size);
void assert_print_caution(char *msg, const char *fname, const char *func, uint32 line);
void assert_print_exit(char *msg, const char *fname,const char *func, uint32 line);
void memory_track_counters_print(void);
void memory_track_free(void *ptr);

#endif
