void *
memory_track_melloc(uint32 size)
{
    malloc_cnt++;
    return malloc(size);
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
    dp_printf("Malloc counters: %d\n", malloc_cnt);
    dp_printf("Free   counters: %d\n", free_cnt);
}

