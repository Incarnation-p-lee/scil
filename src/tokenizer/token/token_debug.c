static inline bool
token_language_c_keyword_legal_p(char *keyword)
{
    e_token_language_c_kywd_type_t type;

    assert_exit(keyword);

    type = TK_4_CHAR_TO_U32(keyword);

    switch (type) {
        case TK_C_KYWD_ASM:
        case TK_C_KYWD_AUTO:
        case TK_C_KYWD_BREAK:
        case TK_C_KYWD_CASE:
        case TK_C_KYWD_CHAR:
        case TK_C_KYWD_CONST:
        case TK_C_KYWD_CONTINUE:
        case TK_C_KYWD_DEFAULT:
        case TK_C_KYWD_DOUBLE:
        case TK_C_KYWD_DO:
        case TK_C_KYWD_ELSE:
        case TK_C_KYWD_ENUM:
        case TK_C_KYWD_EXTERN:
        case TK_C_KYWD_FLOAT:
        case TK_C_KYWD_FOR:
        case TK_C_KYWD_GOTO:
        case TK_C_KYWD_IF:
        case TK_C_KYWD_INLINE:
        case TK_C_KYWD_INT:
        case TK_C_KYWD_LONG:
        case TK_C_KYWD_REGISTER:
        case TK_C_KYWD_RESTRICT:
        case TK_C_KYWD_RETURN:
        case TK_C_KYWD_SHORT:
        case TK_C_KYWD_SIGNED:
        case TK_C_KYWD_SIZEOF:
        case TK_C_KYWD_STATIC:
        case TK_C_KYWD_STRUCT:
        case TK_C_KYWD_SWITCH:
        case TK_C_KYWD_TYPEDEF:
        case TK_C_KYWD_UNIONEF:
        case TK_C_KYWD_UNSIGNED:
        case TK_C_KYWD_VOID:
        case TK_C_KYWD_VOLATILE:
        case TK_C_KYWD_WHILE:
            return true;
        default:
            return false;
    }
}

static inline void
token_language_c_print(s_token_t *token)
{
    assert_exit(token_structure_legal_p(token));

    switch (token->type) {
        case TK_LEX_HEAD:
            scil_log_print(" >> token HEAD\n");
            break;
        case TK_LEX_OPTR:
            scil_log_print(" >> token OPTR\n");
            break;
        case TK_LEX_KWRD:
            scil_log_print(" >> token KWRD\n");
            break;
        case TK_LEX_IDTR:
            scil_log_print(" >> token IDTR\n");
            break;
        case TK_LEX_CNST:
            scil_log_print(" >> token CNST\n");
            break;
        case TK_LEX_PCTT:
            scil_log_print(" >> token PCTT\n");
            break;
        default:
            assert_exit(false);
            break;
    }
}

