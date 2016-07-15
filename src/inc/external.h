#ifndef HAVE_DEFINED_EXTERNAL_H
#define HAVE_DEFINED_EXTERNAL_H

extern s_nfa_t * nfa_engine_create(char *re);
extern void * memory_track_melloc(uint32 size);
extern void complain_assert_caution(char *msg, const char *fname,const char *func, uint32 line);
extern void complain_assert_exit(char *msg, const char *fname,const char *func, uint32 line);
extern void memory_track_counters_print(void);
extern void memory_track_free(void *ptr);
extern void nfa_engine_destroy(s_nfa_t *nfa);
extern void test_main(void);

#endif

