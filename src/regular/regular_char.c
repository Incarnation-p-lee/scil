bool
regular_char_translated_p(char c)
{
    if (TRANS_MASK == (c & TRANS_MASK)) {
        return true;
    } else {
        return false;
    }
}

char
regular_char_translate_resume(char c)
{
    if (!regular_char_translated_p(c)) {
        return c;
    } else if (!regular_char_wildcard_p(c & TRANS_UNMASK)) {
        return c;
    } else {
        return c & TRANS_UNMASK;
    }
}

bool
regular_char_wildcard_p(char c)
{
    switch (c) {
        case RE_WILD_BKT_L:   /* '(' */
        case RE_WILD_BKT_R:   /* ')' */
        case RE_WILD_AND:     /* '&' */
        case RE_WILD_OR:      /* '|' */
        case RE_WILD_STAR:    /* '*' */
        case RE_WILD_PLUS:    /* '+' */
        case RE_WILD_QUST:    /* '?' */
        case RE_WILD_MBKT_L:  /* '[' */
        case RE_WILD_MBKT_R:  /* ']' */
        case RE_WILD_TRANS:   /* '`' */
            return true;
        default:
            return false;
    }
}

static inline bool
regular_char_data_extra_p(char c)
{
    switch (c) {
        case RE_DT_SML_B_SLASH_CHAR:    /* '\' */
        case RE_DT_SML_COLON_CHAR:      /* ':' */
        case RE_DT_SML_COMMA_CHAR:      /* ',' */
        case RE_DT_SML_D_QUOTE_CHAR:    /* '"' */
        case RE_DT_SML_DOT_CHAR:        /* '.' */
        case RE_DT_SML_EQ_CHAR:         /* '=' */
        case RE_DT_SML_GT_CHAR:         /* '>' */
        case RE_DT_SML_LT_CHAR:         /* '<' */
        case RE_DT_SML_L_BRACE_L:       /* '{' */
        case RE_DT_SML_L_BRACE_R:       /* '}' */
        case RE_DT_SML_NOT_CHAR:        /* '!' */
        case RE_DT_SML_PERCENT_CHAR:    /* '%' */
        case RE_DT_SML_POUND_CHAR:      /* '#' */
        case RE_DT_SML_SEMI_CHAR:       /* ';' */
        case RE_DT_SML_SLASH_CHAR:      /* '/' */
        case RE_DT_SML_SPACE_CHAR:      /* ' ' */
        case RE_DT_SML_SUB_CHAR:        /* '-' */
        case RE_DT_SML_TILDE_CHAR:      /* '~' */
        case RE_DT_SML_UDRLINE_CHAR:    /* '_' */
        case RE_DT_SML_XOR_CHAR:        /* '^' */
            return true;
        default:
            return false;
    }
}

bool
regular_char_data_p(char c)
{
    if (dp_isalnum(c)) {
        return true;
    } else if (regular_char_translated_p(c)) {
        return true;
    } else {
        return regular_char_data_extra_p(c);
    }
}

bool
regular_char_bracket_left_p(char c)
{
    if (RE_WILD_BKT_L == c) {
        return true;
    } else {
        return false;
    }
}

bool
regular_char_bracket_right_p(char c)
{
    if (RE_WILD_BKT_R == c) {
        return true;
    } else {
        return false;
    }
}

bool
regular_char_bracket_p(char c)
{
    return regular_char_bracket_left_p(c) || regular_char_bracket_right_p(c);
}

bool
regular_char_wildcard_unary_p(char c)
{
    switch (c) {
        case RE_WILD_STAR:
        case RE_WILD_PLUS:
        case RE_WILD_QUST:
            return true;
        default:
            return false;
    }
}

bool
regular_char_wildcard_binary_p(char c)
{
    switch (c) {
        case RE_WILD_OR:
        case RE_WILD_AND:
            return true;
        default:
            return false;
    }
}

