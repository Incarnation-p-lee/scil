#include "depends.h"

sint32
main(sint32 argc, char **argv)
{
    scil_log_initial();
    libds_log_file_create();

    // test_main();

    memory_cache_cleanup();
    memory_track_counters_print();

    libds_log_file_close();
    scil_log_close();

    return 0;
}



