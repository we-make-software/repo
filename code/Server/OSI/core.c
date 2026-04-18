#include "../.setup"

InitLibrary(Server,OSI)
{
    LibraryInit(Server,OSI,EUI48)
    LibraryInit(Server,OSI,IEEE8023)
    LibraryInit(Server,OSI,RFC2464)

}

QuitLibrary(Server,OSI)
{
    LibraryQuit(Server,OSI,RFC2464)
    LibraryQuit(Server,OSI,IEEE8023)
    LibraryQuit(Server,OSI,EUI48)
}
