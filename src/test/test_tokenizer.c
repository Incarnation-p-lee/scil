static inline void
test_tokenizer_nfa_engine(void)
{
    s_nfa_t *nfa;

    nfa = nfa_engine_create("a|b");
    assert_caution(nfa_engine_pattern_match_p(nfa, "a"));
    assert_caution(nfa_engine_pattern_match_p(nfa, "b"));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "c"));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "ab"));
    nfa_engine_destroy(nfa);

    nfa = nfa_engine_create("ab");
    assert_caution(!nfa_engine_pattern_match_p(nfa, "a"));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "b"));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "c"));
    assert_caution(nfa_engine_pattern_match_p(nfa, "ab"));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "abc"));
    nfa_engine_destroy(nfa);

    nfa = nfa_engine_create("a*");
    assert_caution(nfa_engine_pattern_match_p(nfa, "a"));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "b"));
    assert_caution(nfa_engine_pattern_match_p(nfa, "aa"));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "ab"));
    assert_caution(nfa_engine_pattern_match_p(nfa, "aaaaaaa"));
    nfa_engine_destroy(nfa);
}

static inline void
test_tokenizer_main(void)
{
    test_tokenizer_nfa_engine();
}

