bool
tk_char_double_quote_p(char c)
{
    if (TK_DOUBLE_QUOTE == c) {
        return true;
    } else {
        return false;
    }
}

bool
tk_char_comment_p(char *buf, e_tkz_lang_type_t tkz_type)
{
    if (!buf) {
        return false;
    } else {
        return tk_char_single_comment_p(buf, tkz_type)
            || tk_char_multiple_comment_head_p(buf, tkz_type);
    }
}

bool
tk_char_single_comment_p(char *buf, e_tkz_lang_type_t tkz_type)
{
    if (!buf) {
        return false;
    } else {
        switch (tkz_type) {
            case TKZ_LANG_C:
                return tk_lang_c_single_comment_p(buf);
            default:
                return false;
        }
    }
}

bool
tk_char_multiple_comment_head_p(char *buf, e_tkz_lang_type_t tkz_type)
{
    if (!buf) {
        return false;
    } else {
        switch (tkz_type) {
            case TKZ_LANG_C:
                return tk_lang_c_multiple_comment_head_p(buf);
            default:
                return false;
        }
    }
}

bool
tk_char_multiple_comment_tail_p(char *buf, e_tkz_lang_type_t tkz_type)
{
    if (!buf) {
        return false;
    } else {
        switch (tkz_type) {
            case TKZ_LANG_C:
                return tk_lang_c_multiple_comment_tail_p(buf);
            default:
                return false;
        }
    }
}

