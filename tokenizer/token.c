static inline void
token_scan_line_blank_remove(char *buf, int size);
static inline void
token_scan_line(char *buf, struct token *head);
static struct token *
token_next(struct token *head);

struct token *
token_scan(char *fname)
{
    FILE *fn;
    char *buf;
    struct token *head;

    if (fname) {
        fn = fopen(fname, "r");
        if (fn) {
            buf = malloc(sizeof(char) * BUF_LIMIT);
            if (buf) {
                head = malloc(sizeof(*head));
                head->type = HEAD;
                doubly_linked_list_initial(&head->list);

                while (buf == fgets(buf, BUF_LIMIT, fn)) {
                    token_scan_line_blank_remove(buf, BUF_LIMIT);
                    token_scan_line(buf, head);
                }
            }

            free(buf);
        }
    }

    return head;
}

void
token_print(struct token *head)
{
    struct token *l;

    assert(head);
    assert(head->type == HEAD);

    l = token_next(head);

    while (l != head) {
        switch (l->type) {
            case IDT:
                printf("<%s, '%s'>", TOSTRING(IDT), l->name);
                break;
            case NUM:
                printf("<%s, %d>", TOSTRING(NUM), l->cst);
                break;
            case STR:
                printf("<%s, %s>", TOSTRING(STR), l->str);
                break;
            case OPT:
                printf("<%s, '%s'>", TOSTRING(OPT), l->name);
                break;
            case KWD:
                printf("<%s, '%s'>", TOSTRING(KWD), l->name);
                break;
            case STE:
                printf("<;>\n");
                break;
            default:
                assert(false);
                break;
        }

        l = token_next(l);
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
}

static struct token *
token_next(struct token *head)
{
    void *retval;
    struct doubly_linked_list *list;

    assert(NULL != head);

    list = &head->list;
    list = list->next;

    retval = (void *)((void *)list - (void *)(&((struct token *)0)->list));

    return (struct token *)retval;
}

#if 0
static struct token *
token_preivous(struct token *head)
{
    void *retval;
    struct doubly_linked_list *list;

    assert(NULL != head);

    list = &head->list;
    list = list->previous;

    retval = (void *)((void *)list - (void *)(&((struct token *)0)->list));

    return (struct token *)retval;
}
#endif

static inline bool
isletter(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static inline bool
isidentifier(char c)
{
    return isletter(c) || isdigit(c);
}

static inline uint32
token_scan_number(char *buf, struct token *head)
{
    char *c;
    sint32 val;
    struct token *t;

    assert(buf);
    assert(isdigit(*buf));
    assert(head);

    c = buf;
    val = 0;

    while (isdigit(*c)) {
        val = 10 * val + (int)(*c - '0');
        c++;
    }

    t = malloc(sizeof(*t));
    t->type = NUM;
    t->cst = val;
    doubly_linked_list_insert_ptr_before(&head->list, &t->list);

    return (uint32)(c - buf);
}

static inline bool
isoperator(char c)
{
    if (c == '=') {
        return true;
    } else if (c == '+' || c == '-' || c == '*' || c == '/') {
        return true;
    } else if (c == '|' || c == '&' || c == '>' || c == '<') {
        return true;
    } else if (c == '!' || c == '^' || c == '#') {
        return true;
    } else {
        return false;
    }
}

static inline bool
token_keyword_p(char *t)
{
    char **tmp;

    assert(t);

    tmp = (char **)keyword;

    while (*tmp) {
        if (!strcmp(t, *tmp)) {
            return true;
        }
        tmp++;
    }

    return false;
}

static inline uint32
token_scan_identifier(char *buf, struct token *head)
{
    char *c;
    uint32 i;
    bool is_keyword;
    struct token *t;
    char tmp[NAME_LIMIT];

    assert(buf);
    assert(isletter(*buf));
    assert(head);

    c = buf;
    i = 0;

    while (isidentifier(*c)) {
        tmp[i++] = *c++;
    }

    assert(i < NAME_LIMIT);

    tmp[i] = CHR_NULL;
    is_keyword = token_keyword_p(tmp);
    t = malloc(sizeof(*t));

    t->type = is_keyword ? KWD : IDT;
    strcpy(t->name, tmp);
    doubly_linked_list_insert_ptr_before(&head->list, &t->list);

    return (uint32)(c - buf);
}

static inline uint32
token_scan_operator(char *buf, struct token *head)
{
    char *c;
    uint32 i;
    struct token *t;
    char tmp[OPT_LIMIT];

    assert(buf);
    assert(head);
    assert(isoperator(*buf));

    c = buf;
    i = 0;

    while (isoperator(*c)) {
        tmp[i++] = *c;
        if ('=' == *c) {
            c++;
            break;
        }
        c++;
    }

    tmp[i] = CHR_NULL;
    assert(i < OPT_LIMIT);

    t = malloc(sizeof(*t));
    t->type = OPT;
    strcpy(t->name, tmp);
    doubly_linked_list_insert_ptr_before(&head->list, &t->list);

    return (uint32)(c - buf);
}

static inline void
token_scan_line(char *buf, struct token *head)
{
    char *c;
    struct token *t;

    assert(buf);
    assert(head);

    c = buf;

    while (*c) {
        if (isdigit(*c)) {
            c += token_scan_number(c, head);
        }

        if (isletter(*c)) {
            c += token_scan_identifier(c, head);
        }

        if (isoperator(*c)) {
            c += token_scan_operator(c, head);
        }

        if (';' == *c) {
            t = malloc(sizeof(*t));
            t->type = STE;
            doubly_linked_list_insert_ptr_before(&head->list, &t->list);
            c++;
        }
    }
}

