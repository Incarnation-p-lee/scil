#ifndef COMMON_DECLARATION_H
#define COMMON_DECLARATION_H


void * memory_track_melloc(uint32 size);
void complain_assert_caution(char *msg, const char *fname,const char *func, uint32 line);
void complain_assert_exit(char *msg, const char *fname,const char *func, uint32 line);
void memory_track_counters_print(void);
void memory_track_free(void *ptr);

#endif
