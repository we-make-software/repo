#include "../.setup"
#include "IEEE8023/.setup"
#include "RFC2464/.setup"
#include "RFC7042/.setup"
#include "RFC791/.setup"
#include "RFC8200/.setup"
#include "RFC826/.setup"
#include "RFC4443/.setup"

InitLibrary(Server,OSI)
{
    LibraryInit(Server,OSI,RFC2464)
    LibraryInit(Server,OSI,RFC7042)
    LibraryInit(Server,OSI,IEEE8023)
    LibraryInit(Server,OSI,RFC791)
    LibraryInit(Server,OSI,RFC8200)
    LibraryInit(Server,OSI,RFC826)
    LibraryInit(Server,OSI,RFC4443)
}

QuitLibrary(Server,OSI)
{
    LibraryQuit(Server,OSI,RFC4443)
    LibraryQuit(Server,OSI,RFC826)
    LibraryQuit(Server,OSI,RFC8200)
    LibraryQuit(Server,OSI,RFC791)
    LibraryQuit(Server,OSI,IEEE8023)
    LibraryQuit(Server,OSI,RFC7042)
    LibraryQuit(Server,OSI,RFC2464)
}
