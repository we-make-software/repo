#pragma once

#include ".setup"
#include "../network/.inc"

InitFunction(core)
{
    network_init();
    /*  core init code */
}

ExitFunction(core)
{
    network_exit();
    /* core exit code */
}