static inline void
token_language_c_log_print(s_token_t *token)
{
    s_token_language_c_idtr_t *idtr;
    s_token_language_c_optr_t *optr;
    s_token_language_c_pctt_t *pctt;
    s_token_language_c_cnst_t *cnst;

    assert_exit(token_structure_legal_p(token));

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
token_language_c_pctt_char_p(char c)
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

