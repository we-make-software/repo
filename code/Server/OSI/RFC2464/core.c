#include ".setup"
#include "../RFC8200/.setup"
#include "../RFC4443/.setup"
#include "../../Debug/.setup"

DebugOutput(Server,OSI,RFC2464);
DebugMessages(Universal_ICMPv6,Server,OSI,RFC2464);
DebugMessages(Local_ICMPv6,Server,OSI,RFC2464);

Fn(bool,Server,OSI,RFC2464,Is,Multicast)(StructAlign(Server,OSI,EUI48)*soe)
{
    return(soe->data.bytes[0]==51&&soe->data.bytes[1]==51);
}

static FnRX(Server,OSI,RFC2464,Universal)(struct sk_buff*skb,StructMemory(Server,Hardware,Network)*shn,StructAlign(Server,OSI,IEEE8023)*soi,StructAlign(Server,OSI,RFC8200)*sor)
{
    StructAlign(Server,OSI,RFC4443)*sic=(void*)(sor+1);
    u8 buf[10];
    snprintf(buf,10,"%u,%u",sic->type,sic->code);
    DebugValuePrepare(Universal_ICMPv6,buf,Server,OSI,RFC2464)
}

static FnRX(Server,OSI,RFC2464,Local)(struct sk_buff*skb,StructMemory(Server,Hardware,Network)*shn,StructAlign(Server,OSI,IEEE8023)*soi,StructAlign(Server,OSI,RFC8200)*sor)
{
    StructAlign(Server,OSI,RFC4443)*sic=(void*)(sor+1);
    u8 buf[10];
    snprintf(buf,10,"%u,%u",sic->type,sic->code);
    DebugValuePrepare(Local_ICMPv6,buf,Server,OSI,RFC2464)
}

FnRX(Server,OSI,RFC2464,Universal,Or,Local,Choice)(struct sk_buff*skb,StructMemory(Server,Hardware,Network)*shn,StructAlign(Server,OSI,IEEE8023)*soi)
{
    (soi->Source.data.information.ul?RX(Server,OSI,RFC2464,Local):RX(Server,OSI,RFC2464,Universal))(skb,shn,soi,(void*)(soi+1));
}

InitLibrary(Server,OSI,RFC2464)
{
    DebugOutputPrepare(Server,OSI,RFC2464);
    DebugMessagesPrepare(Universal_ICMPv6,Server,OSI,RFC2464)
    DebugMessagesPrepare(Local_ICMPv6,Server,OSI,RFC2464)
}
