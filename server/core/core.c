#include ".setup"
#include ".init"
InitAndExitFunction(network);

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