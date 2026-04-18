#include ".setup"
#include "../../Debug/.setup"

DebugOutput(Server,OSI,IEEE8023);
DebugMessages(IEEE8023_Type,Server,OSI,IEEE8023);

FnRX(Server,OSI,IEEE8023,Type,Choice)(struct sk_buff*skb,StructMemory(Server,Hardware,Network)*shn,StructAlign(Server,OSI,IEEE8023)*soi)
{
    u8 type[6];
    snprintf(type,6,"%u",be16_to_cpu(soi->Type));
    DebugValuePrepare(IEEE8023_Type,type,Server,OSI,IEEE8023)
}

InitLibrary(Server,OSI,IEEE8023)
{
    DebugOutputPrepare(Server,OSI,IEEE8023);
    DebugMessagesPrepare(IEEE8023_Type,Server,OSI,IEEE8023)
}

QuitLibrary(Server,OSI,IEEE8023)
{
}
