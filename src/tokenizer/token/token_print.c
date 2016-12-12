void
tk_print(s_tk_t *token, e_tkz_lang_type_t tkz_lang_type)
{
    if (tk_structure_legal_p(token)) {
        switch (tkz_lang_type) {
            case TKZ_LANG_C:
                tk_lang_c_print(token);
                break;
            default:
                log_print_and_exit("Unknown lang type.\n");
                break;
        }
    }
}

static inline void
tk_lang_c_print(s_tk_t *token)
{
    uint32 line_nmbr;

    s_tk_lang_c_idtr_t *idtr;
    s_tk_lang_c_optr_t *optr;
    s_tk_lang_c_pctt_t *pctt;
    s_tk_lang_c_cnst_t *cnst;

    assert_exit(tk_structure_legal_p(token));

    line_nmbr = token->line_nmbr;

    switch (token->type) {
        case TK_LEX_HEAD:
            dp_printf("HEAD\n");
            break;
        case TK_LEX_OPTR:
            optr = token->data;
            dp_printf("OPTR %d %s\n", line_nmbr, optr->name);
            break;
        case TK_LEX_KWRD:
            idtr = token->data;
            dp_printf("KWRD %d %s\n", line_nmbr, idtr->name);
            break;
        case TK_LEX_IDTR:
            idtr = token->data;
            dp_printf("IDTR %d %s\n", line_nmbr, idtr->name);
            break;
        case TK_LEX_CNST:
            cnst = token->data;
            dp_printf("CNST %d %s\n", line_nmbr, cnst->name);
            break;
        case TK_LEX_PCTT:
            pctt = token->data;
            dp_printf("PCTT %d %c\n", line_nmbr, pctt->c);
            break;
        default:
            assert_exit(false);
            break;
    }

}

