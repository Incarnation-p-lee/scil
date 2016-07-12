static inline void
test_tokenizer_main(void)
{
    // should be replaced as test assert and will not abort
    assert(test_tokenizer_nfa_engine_create());
}

static inline bool
test_tokenizer_nfa_engine_create(void)
{
    s_nfa_t *nfa;

    nfa = nfa_engine_create("ab(a|b)*");

    nfa_engine_destroy(nfa);

    return true;
}

