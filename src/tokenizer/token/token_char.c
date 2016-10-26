bool
token_char_double_quote_p(char c)
{
    if (TK_DOUBLE_QUOTE == c) {
        return true;
    } else {
        return false;
    }
}

bool
token_char_single_comment_p(char *buf, e_tokenizer_language_type_t tkz_type)
{
    if (!buf) {
        return false;
    } else {
        switch (tkz_type) {
            case TKZ_LANG_C:
                return token_language_c_single_comment_p(buf);
            default:
                return false;
        }
    }
}

bool
token_char_multiple_comment_head_p(char *buf, e_tokenizer_language_type_t tkz_type)
{
    if (!buf) {
        return false;
    } else {
        switch (tkz_type) {
            case TKZ_LANG_C:
                return token_language_c_multiple_comment_head_p(buf);
            default:
                return false;
        }
    }
}

bool
token_char_multiple_comment_tail_p(char *buf, e_tokenizer_language_type_t tkz_type)
{
    if (!buf) {
        return false;
    } else {
        switch (tkz_type) {
            case TKZ_LANG_C:
                return token_language_c_multiple_comment_tail_p(buf);
            default:
                return false;
        }
    }
}
