void
token_print(s_token_t *token, e_tokenizer_language_type_t tkz_language_type)
{
    if (token_structure_legal_p(token)) {
        switch (tkz_language_type) {
            case TKZ_LANG_C:
                token_language_c_print(token);
                break;
            default:
                scil_log_print_and_exit("Unknown language type.\n");
                break;
        }
    }
}

static inline void
token_language_c_print(s_token_t *token)
{
    s_token_language_c_idtr_t *idtr;
    s_token_language_c_optr_t *optr;
    s_token_language_c_pctt_t *pctt;
    s_token_language_c_cnst_t *cnst;

    assert_exit(token_structure_legal_p(token));

    switch (token->type) {
        case TK_LEX_HEAD:
            dp_printf("HEAD\n");
            break;
        case TK_LEX_OPTR:
            optr = token->data;
            dp_printf("OPTR %s\n", optr->name);
            break;
        case TK_LEX_KWRD:
            idtr = token->data;
            dp_printf("KWRD %s\n", idtr->name);
            break;
        case TK_LEX_IDTR:
            idtr = token->data;
            dp_printf("IDTR %s\n", idtr->name);
            break;
        case TK_LEX_CNST:
            cnst = token->data;
            dp_printf("CNST %s\n", cnst->name);
            break;
        case TK_LEX_PCTT:
            pctt = token->data;
            dp_printf("PCTT %c\n",
                token_language_c_pctt_type_to_char(pctt->type));
            break;
        default:
            assert_exit(false);
            break;
    }
}

