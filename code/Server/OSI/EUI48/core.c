#include ".setup"
#include "../RFC7042/.setup"
#include "../RFC2464/.setup"
#include "../IEEE8023/.setup"

FnRX(Server,OSI,EUI48,Destination)(struct sk_buff*skb,StructMemory(Server,Hardware,Network)*shn,StructAlign(Server,OSI,EUI48)*soe)
{
    if(Call(Server,OSI,RFC7042,Is,Broadcast)(soe))
    {
        RX(Server,OSI,RFC7042,Universal,Or,Local,Choice)(skb,shn,(void*)soe);
        return;
    }
    if(Call(Server,OSI,RFC2464,Is,Multicast)(soe))
    {
        RX(Server,OSI,RFC2464,Universal,Or,Local,Choice)(skb,shn,(void*)soe);
        return;
    }
    RX(Server,OSI,IEEE8023,Type,Choice)(skb,shn,(void*)soe);
}

InitLibrary(Server,OSI,EUI48)
{
}

QuitLibrary(Server,OSI,EUI48)
{
}
