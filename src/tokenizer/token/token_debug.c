void
tk_log_print(s_tk_t *token)
{
    uint32 line_nmbr;
    s_tk_lang_c_idtr_t *idtr;
    s_tk_lang_c_optr_t *optr;
    s_tk_lang_c_pctt_t *pctt;
    s_tk_lang_c_cnst_t *cnst;

    RETURN_IF_FALSE(tk_structure_legal_p(token));
    RETURN_IF_FALSE(log_option_token_verbose_p());

    line_nmbr = token->line_nmbr;

    log_print("[TK] token ");

    switch (token->type) {
        case TK_LEX_HEAD:
            log_print("HEAD %s\n", token->data);
            break;
        case TK_LEX_OPTR:
            optr = token->data;
            log_print("OPTR %d %s\n", line_nmbr, optr->name);
            break;
        case TK_LEX_KWRD:
            idtr = token->data;
            log_print("KWRD %d %s\n", line_nmbr, idtr->name);
            break;
        case TK_LEX_IDTR:
            idtr = token->data;
            log_print("IDTR %d %s\n", line_nmbr, idtr->name);
            break;
        case TK_LEX_CNST:
            cnst = token->data;
            log_print("CNST %d %s\n", line_nmbr, cnst->name);
            break;
        case TK_LEX_PCTT:
            pctt = token->data;
            log_print("PCTT %d %c\n", line_nmbr, pctt->c);
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

    RETURN_IF_FALSE(log_option_token_verbose_p());

    log_print("[TK] try identifier on '%s'\n", buf);
}

static inline void
tk_lang_c_optr_print(char *buf)
{
    assert_exit(buf);

    RETURN_IF_FALSE(log_option_token_verbose_p());

    log_print("[TK] try operator on '%s'\n", buf);
}

static inline void
tk_lang_c_cnst_print(char *buf)
{
    assert_exit(buf);

    RETURN_IF_FALSE(log_option_token_verbose_p());

    log_print("[TK] try constant on '%s'\n", buf);
}

static inline void
tk_lang_c_pctt_print(char *buf)
{
    assert_exit(buf);

    RETURN_IF_FALSE(log_option_token_verbose_p());

    log_print("[TK] try punctuation on '%s'\n", buf);
}

