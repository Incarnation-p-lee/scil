void
complain_assert(char *msg, char *fname, char *func, uint32 line)
{
    if (msg && fname && func && line) {
        dp_printf("COMPLAIN ASSERT: => < %s > Fail ...\n"
                  "  at function %s\n"
                  "  in file %s +%d\n", msg, func, fname, line);
    }
}


