#include "depends.h"

sint32
main(sint32 argc, char **argv)
{
    test_main();

    memory_track_counters_print();
    return 0;
}


