void *
memory_track_malloc(uint32 size)
{
    malloc_cnt++;
    return malloc(size);
}

void *
memory_track_realloc(void *ptr, uint32 size)
{
    if (!ptr) {
        return NULL;
    } else {
        realloc_cnt++;
        return realloc(ptr, size);
    }
}

void
memory_track_free(void *ptr)
{
    free_cnt++;
    free(ptr);
}

void
memory_track_counters_print(void)
{
    dp_printf("  >> SCIL Memory Usage Summary <<\n");
    dp_printf("    . Malloc  %10d\n", malloc_cnt);
    dp_printf("    . Realloc %10d\n", realloc_cnt);
    dp_printf("    . Free    %10d\n", free_cnt);
    dp_printf("  >> Memory End <<\n\n");

    if (free_cnt != malloc_cnt) {
        dp_printf("Memory leak at dp_malloc %10d <=> dp_free %10d\n",
            malloc_cnt, free_cnt);
        dp_exit(1);
    }
}

