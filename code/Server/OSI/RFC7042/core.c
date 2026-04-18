#include ".setup"
#include "../../Debug/.setup"

DebugOutput(Server,OSI,RFC7042);
DebugMessages(IEEE8023_Universal_Type,Server,OSI,RFC7042);
DebugMessages(IEEE8023_Local_Type,Server,OSI,RFC7042);

Fn(bool,Server,OSI,RFC7042,Is,Broadcast)(StructAlign(Server,OSI,EUI48)*soe)
{
    return(soe->data.bytes[0]==255&&soe->data.bytes[1]==255&&soe->data.bytes[2]==255&&soe->data.bytes[3]==255&&soe->data.bytes[4]==255&&soe->data.bytes[5]==255);
}

static FnRX(Server,OSI,RFC7042,Universal,Type,Choice)(struct sk_buff*skb,StructMemory(Server,Hardware,Network)*shn,StructAlign(Server,OSI,IEEE8023)*soi)
{
    u8 type[6];
    snprintf(type,6,"%u",be16_to_cpu(soi->Type));
    DebugValuePrepare(IEEE8023_Universal_Type,type,Server,OSI,RFC7042)
}

static FnRX(Server,OSI,RFC7042,Local,Type,Choice)(struct sk_buff*skb,StructMemory(Server,Hardware,Network)*shn,StructAlign(Server,OSI,IEEE8023)*soi)
{
    u8 type[6];
    snprintf(type,6,"%u",be16_to_cpu(soi->Type));
    DebugValuePrepare(IEEE8023_Local_Type,type,Server,OSI,RFC7042)
}

FnRX(Server,OSI,RFC7042,Universal,Or,Local,Choice)(struct sk_buff*skb,StructMemory(Server,Hardware,Network)*shn,StructAlign(Server,OSI,IEEE8023)*soi)
{
    (soi->Source.data.information.ul?RX(Server,OSI,RFC7042,Local,Type,Choice):RX(Server,OSI,RFC7042,Universal,Type,Choice))(skb,shn,soi);
}

InitLibrary(Server,OSI,RFC7042)
{
    DebugOutputPrepare(Server,OSI,RFC7042);
    DebugMessagesPrepare(IEEE8023_Universal_Type,Server,OSI,RFC7042)
    DebugMessagesPrepare(IEEE8023_Local_Type,Server,OSI,RFC7042)
}

QuitLibrary(Server,OSI,RFC7042)
{
}
