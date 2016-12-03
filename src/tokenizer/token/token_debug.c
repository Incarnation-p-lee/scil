static inline void
tk_lang_c_log_print(s_tk_t *token)
{
    s_tk_lang_c_idtr_t *idtr;
    s_tk_lang_c_optr_t *optr;
    s_tk_lang_c_pctt_t *pctt;
    s_tk_lang_c_cnst_t *cnst;

    assert_exit(tk_structure_legal_p(token));

    scil_log_print("!! token ");

    switch (token->type) {
        case TK_LEX_HEAD:
            scil_log_print("HEAD\n");
            break;
        case TK_LEX_OPTR:
            optr = token->data;
            scil_log_print("OPTR %s\n", optr->name);
            break;
        case TK_LEX_KWRD:
            idtr = token->data;
            scil_log_print("IDTR -> KWRD %s\n", idtr->name);
            break;
        case TK_LEX_IDTR:
            idtr = token->data;
            scil_log_print("IDTR %s\n", idtr->name);
            break;
        case TK_LEX_CNST:
            cnst = token->data;
            scil_log_print("CNST %s\n", cnst->name);
            break;
        case TK_LEX_PCTT:
            pctt = token->data;
            scil_log_print("PCTT %c\n", pctt->c);
            break;
        default:
            assert_exit(false);
            break;
    }
}

static inline bool
tk_lang_c_pctt_char_p(char c)
{
    switch (c) {
        case ',':
        case '(':
        case ')':
        case '[':
        case ']':
        case '{':
        case '}':
        case ';':
            return true;
        default:
            return false;
    }
}

static inline void
tk_lang_c_idtr_print(char *buf)
{
    assert_exit(buf);

    scil_log_print("== TK try identifier on '%s'\n", buf);
}

static inline void
tk_lang_c_optr_print(char *buf)
{
    assert_exit(buf);

    scil_log_print("== TK try operator on '%s'\n", buf);
}

static inline void
tk_lang_c_cnst_print(char *buf)
{
    assert_exit(buf);

    scil_log_print("== TK try constant on '%s'\n", buf);
}

static inline void
tk_lang_c_pctt_print(char *buf)
{
    assert_exit(buf);

    scil_log_print("== TK try punctuation on '%s'\n", buf);
}

