#include ".setup"

InitLibrary(Server,Hardware)
{
    LibraryInit(Server,Hardware,Memory);
    LibraryInit(Server,Hardware,Storage);
    LibraryInit(Server,Hardware,Network);
}

QuitLibrary(Server,Hardware)
{
    LibraryQuit(Server,Hardware,Network);
    LibraryQuit(Server,Hardware,Storage);
    LibraryQuit(Server,Hardware,Memory);
}
