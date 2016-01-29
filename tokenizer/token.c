static inline void
token_scan_line_blank_remove(char *buf, int size);

void
token_scan(char *fname)
{
    FILE *fn;
    char *buf;

    if (fname) {
        fn = fopen(fname, "r");
        if (fn) {
            buf = malloc(sizeof(char) * BUF_LIMIT);

            if (buf) {
                while (buf == fgets(buf, BUF_LIMIT, fn)) {
                    token_scan_line_blank_remove(buf, BUF_LIMIT);
                }
            }

            free(buf);
        }
    }
}

static inline void
token_scan_line_blank_remove(char *buf, int size)
{
    int i;
    int j;

    assert(buf);

    i = 0;
    j = 0;
    while (i < size || CHR_NULL == buf[i]) {
        if (!isblank(buf[i]) && !isspace(buf[i])) {
            buf[j++] = buf[i];
        }
        i++;
    }

    buf[j] = CHR_NULL;

    printf("%s\n", buf);
}

