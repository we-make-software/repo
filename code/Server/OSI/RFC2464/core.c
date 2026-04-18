#include ".setup"
Fn(bool,Server,OSI,RFC2464,Is,Multicast)(StructAlign(Server,OSI,IEEE8023)*soi)
{
    return soi->Destination.data.byte16[0]==0b0011001100110011;
}
InitLibrary(Server,OSI,RFC2464)
{
}

QuitLibrary(Server,OSI,RFC2464)
{
}