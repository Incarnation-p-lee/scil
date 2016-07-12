#ifndef HAVE_DEFINED_EXTERNAL_H
#define HAVE_DEFINED_EXTERNAL_H

extern s_nfa_t * nfa_engine_create(char *re);
extern void complain_assert(char *msg, char *fname, char *func, uint32 line);
extern void nfa_engine_destroy(s_nfa_t *nfa);
extern void test_main(void);

#endif

