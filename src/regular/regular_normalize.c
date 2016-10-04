static inline char *
regular_normalize(char *re)
{
    char *normal, *expand;

    if (!re) {
        return NULL;
    } else {
        expand = regular_range_expand(re);
        normal = regular_char_and_insert(expand);

        dp_free(expand);
        return normal;
    }
}

/*
 * Convert RE [A-Z] to (A|B|C|...|Z)
 */
static inline char *
regular_range_expand(char *re)
{
    char *expand, *c;
    uint32 size, i, exp;
    char last, start, advanced;

    assert_exit(re);

    i = 0;
    c = re;
    size = dp_strlen(re) * 2 + RE_EXPAND_MIN;
    expand = dp_malloc(sizeof(char) * size);

    while (*c) {
        if (RE_WILD_TRANS == *c) {
            assert_exit(regular_char_wildcard_p(c[1]));
            expand[i++] = c[1] | TRANS_MASK;
            c++;
        } else if (RE_WILD_MBKT_L != *c) {
            expand[i++] = *c;
        } else {
            c++;                                     /* [A */
            expand[i++] = RE_WILD_BKT_L;
            while (RE_WILD_MBKT_R != *c && *c) {
                advanced = c[1];
                if (RE_DT_SML_SUB_CHAR != advanced) {
                    expand[i++] = *c;
                    expand[i++] = RE_WILD_OR;
                } else {                             /* Handle '-' in [], like [A-Z] */
                    start = *c;                      /* [A-Z */
                    last = c[2];
                    exp = (uint32)(last - start) * 2 + RE_EXPAND_MIN;
                    size += exp;
                    expand = dp_realloc(expand, size);
                    while (start <= last) {
                        expand[i++] = start;
                        expand[i++] = RE_WILD_OR;
                        start++;
                    }
                    c += 2;
                }
                c++;
            }
            expand[i - 1] = RE_WILD_BKT_R;           /* overwrite last RE_WILD_OR */
        }
        c++;
    }

    expand[i] = NULL_CHAR;
    REGULAR_RANG_EXPAND_PRINT(expand);

    return expand;
}

static inline bool
regular_char_and_needed_p(char last, char c)
{
    if ((regular_char_wildcard_unary_p(last) && regular_char_bracket_left_p(c))
        || (regular_char_data_p(last) && regular_char_bracket_left_p(c))
        || (regular_char_bracket_right_p(last) && regular_char_bracket_left_p(c))
        || (regular_char_bracket_right_p(last) && regular_char_data_p(c))
        || (regular_char_wildcard_unary_p(last) && regular_char_data_p(c))
        || (regular_char_data_p(last) && regular_char_data_p(c))) {
        return true;
    } else {
        return false;
    }
}

/*
 * Convert RE abc => a&b&c
 */
static inline char *
regular_char_and_insert(char *re)
{
    uint32 index, size;
    char last, *insert, *c;

    assert_exit(re);

    size = dp_strlen(re) * 2 + RE_EXPAND_MIN;
    insert = dp_malloc(size);

    c = re;
    index = 0;
    last = NULL_CHAR;
    while (*c) {
        if (regular_char_and_needed_p(last, *c)) {
            insert[index++] = RE_WILD_AND;
        }
        insert[index++] = last = *c++;

        if (index + 3 >= size) {
            size = size * 2 + RE_EXPAND_MIN;
            insert = dp_realloc(insert, sizeof(char) * size);
        }
    }

    insert[index] = NULL_CHAR;
    REGULAR_OPT_AND_INSERT_PRINT(insert);
    return insert;
}

