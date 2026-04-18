#include ".setup"
#include "../../Debug/.setup"

DebugOutput(Server,OSI,RFC8200);

Fn(bool,Server,OSI,RFC8200,Is,Link,Local)(u128 address)
{
    u8*a=(u8*)&address;
    return(a[0]==254&&(a[1]&192)==128);
}

Fn(bool,Server,OSI,RFC8200,Is,Global,Unicast)(u128 address)
{
    return((((u8*)&address)[0]&224)==32);
}

FnRX(Server,OSI,RFC8200)(struct sk_buff*skb,StructMemory(Server,Hardware,Network)*shn,StructAlign(Server,OSI,IEEE8023)*soi,StructAlign(Server,OSI,RFC8200)*sor)
{
}

InitLibrary(Server,OSI,RFC8200)
{
    DebugOutputPrepare(Server,OSI,RFC8200);
}
