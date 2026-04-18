#include ".setup"

InitLibrary(Server,Common)
{
    LibraryInit(Server,Common,Sync);
}

QuitLibrary(Server,Common)
{
    LibraryQuit(Server,Common,Sync);
}
