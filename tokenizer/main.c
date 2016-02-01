extern struct token * token_scan(char *fname);
extern void token_print(struct token *head);

int
main(int argc, char **argv)
{
    int i;
    struct token *head;

    i = 1;
    while (i < argc) {
        head = token_scan(argv[i++]);
        token_print(head);
    }

    return 0;
}

