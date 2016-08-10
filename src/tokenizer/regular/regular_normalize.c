static inline char *
regular_expression_normalize(char *re)
{
    char *normal, *expand;

    if (!re) {
        return NULL;
    } else {
        expand = regular_expression_range_expand(re);
        normal = regular_expression_opt_and_insert(expand);

        dp_free(expand);
        return normal;
    }
}

/*
 * Convert RE [A-Z] to (A|B|C|...|Z)
 */
static inline char *
regular_expression_range_expand(char *re)
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
        if (RE_M_OPT_MBKT_L != *c) {
            expand[i++] = *c;
        } else {
            c++;                                     /* [A */
            expand[i++] = RE_M_OPT_BKT_L;
            while (RE_M_OPT_MBKT_R != *c) {
                advanced = c[1];
                if (RE_M_OPT_CNNT != advanced) {
                    expand[i++] = *c;
                    expand[i++] = RE_M_OPT_OR;
                } else {                             /* Handle '-' in [], like [A-Z] */
                    start = *c;                      /* [A-Z */
                    last = c[2];
                    exp = (uint32)(last - start) * 2 + RE_EXPAND_MIN;
                    size += exp;
                    expand = dp_realloc(expand, size);
                    while (start <= last) {
                        expand[i++] = start;
                        expand[i++] = RE_M_OPT_OR;
                        start++;
                    }
                    c += 2;
                }
                c++;
            }
            expand[i - 1] = RE_M_OPT_BKT_R;    /* overwrite last RE_M_OPT_OR */
        }
        c++;
    }

    expand[i] = NULL_CHAR;
    return expand;
}

static inline bool
regular_expression_opt_and_needed_p(char last, char c)
{
    if ((regular_opt_unary_p(last) && c == RE_M_OPT_BKT_L)
        || (regular_data_p(last) && RE_M_OPT_BKT_L == c)
        || (last == RE_M_OPT_BKT_R && regular_data_p(c))
        || (regular_opt_unary_p(last) && regular_data_p(c))
        || (regular_data_p(last) && regular_data_p(c))) {
        return true;
    } else {
        return false;
    }
}

/*
 * Convert RE abc => a&b&c
 */
static inline char *
regular_expression_opt_and_insert(char *re)
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
        if (regular_expression_opt_and_needed_p(last, *c)) {
            insert[index++] = RE_M_OPT_AND;
        }
        insert[index++] = last = *c++;

        if (index + 3 >= size) {
            size = size * 2 + RE_EXPAND_MIN;
            insert = dp_realloc(insert, sizeof(char) * size);
        }
    }

    insert[index] = NULL_CHAR;
    return insert;
}

