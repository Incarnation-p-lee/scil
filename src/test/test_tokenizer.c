static inline bool
test_tokenizer_nfa_engine_create_p(void)
{
    s_nfa_t *nfa;

    nfa = nfa_engine_create("(a|b)*abb");

    nfa_engine_destroy(nfa);

    return true;
}

static inline void
test_tokenizer_main(void)
{
    assert_caution(test_tokenizer_nfa_engine_create_p());
}

