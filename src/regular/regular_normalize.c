static inline char *
regular_normalize(char *re)
{
    char *normal;
    uint32 bytes_count;
    s_regular_recover_t *recover;

    assert_exit(re);

    bytes_count = dp_strlen(re) * 2 + RE_RECOVER_MIN;
    recover = regular_normalize_recover_create(bytes_count);

    regular_normalize_range_recover(recover, re);
    normal = regular_normalize_char_and_insert(recover->buf);

    regular_normalize_recover_destroy(recover);

    return normal;
}

static inline s_regular_recover_t *
regular_normalize_recover_create(uint32 bytes_count)
{
    s_regular_recover_t *recover;

    assert_exit(bytes_count >= RE_RECOVER_MIN);

    recover = dp_malloc(sizeof(*recover));
    recover->buf = dp_malloc(bytes_count);

    recover->size = bytes_count;
    recover->index = 0;

    return recover;
}

static inline void
regular_normalize_recover_destroy(s_regular_recover_t *recover)
{
    assert_exit(regular_normalize_recover_structure_legal_p(recover));

    dp_free(recover->buf);
    dp_free(recover);
}

/*
 * Unfolding [A-Z] will ignore any wildcard.
 * Example: [*&|] is not wildcard.
 */
static inline uint32
regular_normalize_range_unfold_i(s_regular_recover_t *recover, char *re)
{
    char *c;
    char tmp_char;
    char start, last;
    uint32 extra_bytes;

    assert_exit(re);
    assert_exit(re[1] == RE_DT_SML_SUB_CHAR);
    assert_exit(regular_normalize_recover_structure_legal_p(recover));

    c = re;   /* 'A-Z' */
    start = c[RE_RANGE_START];
    last = c[RE_RANGE_LAST];

    extra_bytes = (uint32)(last - start) * 2 + RE_RECOVER_MIN;
    recover->size += extra_bytes;
    recover->buf = dp_realloc(recover->buf, recover->size);

    while (start <= last) {
        tmp_char = start;
        if (regular_char_wildcard_p(tmp_char)) {
            tmp_char = regular_char_wildcard_encode(start);
        }

        recover->buf[recover->index++] = tmp_char;
        recover->buf[recover->index++] = RE_WILD_OR;
        start++;
    }

    return RE_RANGE_SIZE;
}

static inline uint32
regular_normalize_range_unfold(s_regular_recover_t *recover, char *re)
{
    char *c, advanced;
    uint32 forward_bytes;

    assert_exit(re);
    assert_exit(re[0] == RE_WILD_MBKT_L);
    assert_exit(regular_normalize_recover_structure_legal_p(recover));

    c = re + 1;  /* skip '[' of [A */
    recover->buf[recover->index++] = RE_WILD_BKT_L;

    while (RE_WILD_MBKT_R != *c && *c) {
        advanced = c[1];
        if (RE_DT_SML_SUB_CHAR != advanced) {
            recover->buf[recover->index++] = *c;
            recover->buf[recover->index++] = RE_WILD_OR;
            c++;
        } else { /* Handle '-' in [], like [A-Z] */
            c += regular_normalize_range_unfold_i(recover, c);
        }
    }

    recover->buf[recover->index - 1] = RE_WILD_BKT_R; /* overwrite last RE_WILD_OR */
    forward_bytes = (uint32)(c - re);

    return forward_bytes;
}

/*
 * Recover will process 2 things at one iteration.
 * 1. Encode wildcard.
 * 2. Convert RE [A-Z] to (A|B|C|...|Z)
 */
static inline void
regular_normalize_range_recover(s_regular_recover_t *recover, char *re)
{
    char *c;

    assert_exit(re);
    assert_exit(regular_normalize_recover_structure_legal_p(recover));

    c = re;
    while (*c) {
        if (*c == RE_WILD_ENCODE) { /* Encode wildard as normal char */
            assert_exit(regular_char_wildcard_p(c[1]));
            recover->buf[recover->index++] = regular_char_wildcard_encode(c[1]);
            c++;
        } else if (*c != RE_WILD_MBKT_L) {
            recover->buf[recover->index++] = *c;
        } else {                   /* *c == RE_WILD_MBKT_L */
            c += regular_normalize_range_unfold(recover, c);
        }
        c++;
    }

    recover->buf[recover->index] = NULL_CHAR;
    REGULAR_RANG_RECOVER_PRINT(recover->buf);
}

/*
 * Convert RE abc => a&b&c
 */
static inline char *
regular_normalize_char_and_insert(char *re)
{
    uint32 index, size;
    char last, *insert, *c;

    assert_exit(re);

    size = dp_strlen(re) * 2 + RE_RECOVER_MIN;
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
            size = size * 2 + RE_RECOVER_MIN;
            insert = dp_realloc(insert, sizeof(char) * size);
        }
    }

    insert[index] = NULL_CHAR;
    REGULAR_OPT_AND_INSERT_PRINT(insert);
    return insert;
}

