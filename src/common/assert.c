void
assert_print_exit(char *msg, const char *fname, const char *func,
    uint32 line)
{
    assert_print_caution(msg, fname, func, line);
    *(uint32 *)0 = 0;
}

void
assert_print_caution(char *msg, const char *fname, const char *func,
    uint32 line)
{
    if (msg && fname && func && line) {
        dp_printf("COMPLAIN ASSERT: => < %s > Fail ...\n"
                  "  at function %s\n"
                  "  in file %s +%d\n", msg, func, fname, line);
    }
}

