#include ".setup"
#include "../../Debug/.setup"

DebugOutput(Server,OSI,RFC791);

InitLibrary(Server,OSI,RFC791)
{
    DebugOutputPrepare(Server,OSI,RFC791);
}

QuitLibrary(Server,OSI,RFC791)
{
}
