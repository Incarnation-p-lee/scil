static inline uint32
token_lang_c_match_operator(s_nfa_engine_t *nfa, s_token_t *token_head,
    char *buf, char sentinel)
{
    char tmp;
    uint32 n;
    s_token_t *token;

    assert_exit(buf);
    assert_exit(token_head);
    assert_exit(nfa_engine_structure_legal_p(nfa));

    n = nfa_engine_token_match(nfa, buf, sentinel);
    if (n) {
        assert_exit(n <= 2);
        tmp = buf[0];
        buf += n;
        if (token_operator_multi_p(tmp)) {
        } else {
            token = dp_malloc(sizeo
        }
    }
}

static inline bool
token_lang_c_optr_multi_p(char c)
{
    return false;
}

static inline bool
token_lang_c_operator_p(char c)
{
    switch (c) {
        case TK_OPTR_NOT:
        case TK_OPTR_MOD:
        case TK_OPTR_AND:
        case TK_OPTR_MUL:
        case TK_OPTR_ADD:
        case TK_OPTR_SUB:
        case TK_OPTR_REF:
        case TK_OPTR_DIV:
        case TK_OPTR_CLN:
        case TK_OPTR_LT:
        case TK_OPTR_EQ:
        case TK_OPTR_GT:
        case TK_OPTR_QST:
        case TK_OPTR_XOR:
        case TK_OPTR_OR:
        case TK_OPTR_NEG:
            return true;
        default:
            return false;
    }
}

static inline s_token_t *
token_lang_c_optr_create(char c)
{
    assert_exit(token_operator_create

}



