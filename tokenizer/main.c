
int
main(int argc, char **argv)
{
    int i;

    i = 1;
    while (i < argc) {
        token_scan(argv[i++]);
    }

    return 0;
}

