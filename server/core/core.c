#include ".setup"
#include ".init"
#include "../network/.init"

InitFunction(core)
{
    Init(network);
    /*  core init code */
}

ExitFunction(core)
{
    Exit(network);
    /* core exit code */
}