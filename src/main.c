#include "depends.h"

sint32
main(sint32 argc, char **argv)
{
    log_initial();

    test_main();

    memory_cache_cleanup();
    memory_track_counters_print();
    log_close();

    return 0;
}


