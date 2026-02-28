#include "core/.h"
#include "network/.h"
void core_init(void)
{
    network_init();
    /*  core init code */
}

void core_exit(void)
{
    network_exit();
    /* core exit code */
}