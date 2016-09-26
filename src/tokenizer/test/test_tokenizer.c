static inline void
test_tokenizer_nfa_engine_basic(void)
{
    TEST_ISOLATOR;
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
    assert_caution(nfa_engine_pattern_match_p(nfa, ""));
    assert_caution(nfa_engine_pattern_match_p(nfa, "a"));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "b"));
    assert_caution(nfa_engine_pattern_match_p(nfa, "aa"));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "ab"));
    assert_caution(nfa_engine_pattern_match_p(nfa, "aaaaaaa"));
    nfa_engine_destroy(nfa);

    nfa = nfa_engine_create("a+");
    assert_caution(!nfa_engine_pattern_match_p(nfa, ""));
    assert_caution(nfa_engine_pattern_match_p(nfa, "a"));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "b"));
    assert_caution(nfa_engine_pattern_match_p(nfa, "aa"));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "ab"));
    assert_caution(nfa_engine_pattern_match_p(nfa, "aaaaaaa"));
    nfa_engine_destroy(nfa);

    nfa = nfa_engine_create("a?");
    assert_caution(nfa_engine_pattern_match_p(nfa, ""));
    assert_caution(nfa_engine_pattern_match_p(nfa, "a"));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "b"));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "aa"));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "ab"));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "aaaaaaa"));
    nfa_engine_destroy(nfa);

    nfa = nfa_engine_create("[a-g]+");
    assert_caution(!nfa_engine_pattern_match_p(nfa, ""));
    assert_caution(nfa_engine_pattern_match_p(nfa, "a"));
    assert_caution(nfa_engine_pattern_match_p(nfa, "b"));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "h"));
    assert_caution(nfa_engine_pattern_match_p(nfa, "abcdefg"));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "abcdeffgh"));
    nfa_engine_destroy(nfa);
}

static inline void
test_tokenizer_nfa_engine_translate(void)
{
    TEST_ISOLATOR;
    s_nfa_t *nfa;

    nfa = nfa_engine_create("\"[a-zA-Z %#0-9\\]+\"");
    assert_caution(nfa_engine_pattern_match_p(nfa, "\"abcd\""));
    assert_caution(nfa_engine_pattern_match_p(nfa, "\"We    are in kmalloc\\n\""));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "\"We_   are in kmalloc\""));
    nfa_engine_destroy(nfa);


    nfa = nfa_engine_create("abcde");
    assert_caution(!nfa_engine_pattern_match_p(nfa, "abcd"));
    assert_caution(nfa_engine_pattern_match_p(nfa, "abcde"));
    nfa_engine_destroy(nfa);

    nfa = nfa_engine_create("`[");
    assert_caution(!nfa_engine_pattern_match_p(nfa, ""));
    assert_caution(nfa_engine_pattern_match_p(nfa, "["));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "``"));
    nfa_engine_destroy(nfa);

    nfa = nfa_engine_create("`*`+`?");
    assert_caution(!nfa_engine_pattern_match_p(nfa, "*"));
    assert_caution(nfa_engine_pattern_match_p(nfa, "*+?"));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "`*`+`?"));
    nfa_engine_destroy(nfa);

    nfa = nfa_engine_create("`(|`)");
    assert_caution(!nfa_engine_pattern_match_p(nfa, "*"));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "()"));
    assert_caution(nfa_engine_pattern_match_p(nfa, "("));
    assert_caution(nfa_engine_pattern_match_p(nfa, ")"));
    nfa_engine_destroy(nfa);
}


static inline void
test_tokenizer_nfa_engine_advance(void)
{
    TEST_ISOLATOR;
    s_nfa_t *nfa;

    nfa = nfa_engine_create("(abc)+(b|e)?");
    assert_caution(!nfa_engine_pattern_match_p(nfa, ""));
    assert_caution(nfa_engine_pattern_match_p(nfa, "abc"));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "ab"));
    assert_caution(nfa_engine_pattern_match_p(nfa, "abcabc"));
    assert_caution(nfa_engine_pattern_match_p(nfa, "abcb"));
    assert_caution(nfa_engine_pattern_match_p(nfa, "abce"));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "abcbe"));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "abca"));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "abcabcabcea"));
    assert_caution(nfa_engine_pattern_match_p(nfa, "abcabcabce"));
    nfa_engine_destroy(nfa);

    nfa = nfa_engine_create("((abcd)+efghi)?(acde+)+");
    assert_caution(!nfa_engine_pattern_match_p(nfa, ""));
    assert_caution(nfa_engine_pattern_match_p(nfa, "acde"));
    assert_caution(nfa_engine_pattern_match_p(nfa, "acdeeacdeee"));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "acdea"));
    assert_caution(nfa_engine_pattern_match_p(nfa, "abcdabcdefghiacde"));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "abcdefghia"));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "abcddefghiacdeee"));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "abcdabcdefghiaa"));
    assert_caution(nfa_engine_pattern_match_p(nfa, "abcdabcdabcdabcdefghiacdeacde"));
    assert_caution(nfa_engine_pattern_match_p(nfa, "acdeeeeeeeeeee"));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "abcdabcdabcdefghiacda"));
    nfa_engine_destroy(nfa);

    nfa = nfa_engine_create("[A-Za-z_][A-Za-z_0-9]*");
    assert_caution(!nfa_engine_pattern_match_p(nfa, ""));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "0scil_log_initial"));
    assert_caution(nfa_engine_pattern_match_p(nfa, "__scil_log_initial__"));
    assert_caution(nfa_engine_pattern_match_p(nfa, "__FUNCTION_TIME_O2__"));
    assert_caution(nfa_engine_pattern_match_p(nfa, "______________"));
    assert_caution(nfa_engine_pattern_match_p(nfa, "_01234567890__"));
    assert_caution(nfa_engine_pattern_match_p(nfa, "abcedefghids__"));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "___=xefghids__"));
    assert_caution(!nfa_engine_pattern_match_p(nfa, "_logile_name+__"));
    assert_caution(nfa_engine_pattern_match_p(nfa, "test_tokenizer_nfa_engin__"));
    nfa_engine_destroy(nfa);
}

static inline void
test_tokenizer_nfa_engine(void)
{
    test_tokenizer_nfa_engine_basic();
    test_tokenizer_nfa_engine_translate();
    test_tokenizer_nfa_engine_advance();
}

static inline void
test_tokenizer_main(void)
{
    test_tokenizer_nfa_engine();
    tokenizer_file_list_process(&TOKEN_AIM_DATA_FILE, 1);
}

