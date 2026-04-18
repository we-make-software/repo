#include ".setup"
#include "../RFC2464/.setup"
DebugOutput(Server,OSI,IEEE8023);
DebugMessages(IEEE8023_Source_RFC2464IsMulticast,Server,OSI,IEEE8023);
DebugMessages(IEEE8023_Source_Broadcast,Server,OSI,IEEE8023);
DebugMessages(IEEE8023_Source_RFC791,Server,OSI,IEEE8023);
DebugMessages(IEEE8023_Source_RFC826,Server,OSI,IEEE8023);
DebugMessages(IEEE8023_Source_RFC8200,Server,OSI,IEEE8023);


FnNew(Server,OSI,IEEE8023)(StructMemory(Server,Hardware,Network)*shn,StructAlign(Server,OSI,IEEE8023)**soi)
{
    struct sk_buff*buff=New(Server,Hardware,Network)(shn);
    if(!buff) return NULL;
    *soi=(void*)skb_put(buff,sizeof(StructAlign(Server,OSI,IEEE8023)));
    memcpy((*soi)->Source.data.byte8,shn->dev->dev_addr,6);
    return buff;
}

Fn(void,Server,OSI,IEEE8023,Set,Destination)(StructAlign(Server,OSI,IEEE8023)*soi,StructAlign(Server,OSI,EUI48)*soe)
{
    memcpy(soi->Destination.data.byte8,soe->data.byte8,6);
}

Fn(void,Server,OSI,IEEE8023,Set,Type)(StructAlign(Server,OSI,IEEE8023)*soi,__be16 type)
{
    soi->Type=type;
}
Fn(bool,Server,OSI,IEEE8023,Is,Broadcast)(StructAlign(Server,OSI,IEEE8023)*soi)
{
    return soi->Destination.data.byte16[0]==0b1111111111111111&&soi->Destination.data.byte16[1]==0b1111111111111111&&soi->Destination.data.byte16[2]==0b1111111111111111;
}

FnRX(Server,OSI,IEEE8023)(struct sk_buff*skb,StructMemory(Server,Hardware,Network)*shn,StructAlign(Server,OSI,IEEE8023)*soi)
{
             char debug_message[18];
    sprintf(debug_message, "%02x:%02x:%02x:%02x:%02x:%02x",
        soi->Source.data.byte8[0], soi->Source.data.byte8[1],
        soi->Source.data.byte8[2], soi->Source.data.byte8[3],
        soi->Source.data.byte8[4], soi->Source.data.byte8[5]);
    if(IEEE8023IsBroadcast(soi))
    {
        DebugValuePrepare(IEEE8023_Source_Broadcast,debug_message,Server,OSI,IEEE8023);
    }
    /*
    
    
        switch(soi->Type)
        {
            case cpu_to_be16(2048):
               // RX(Server,OSI,RFC791,Broadcast)(skb,shn,soi,(void*)(soi+1));
                return;
            default:
                //RX(Server,OSI,RFC826,Broadcast)(skb,shn,soi,(void*)(soi+1));
                return;
        }
*/
    if(RFC2464IsMulticast(soi))
    {
        DebugValuePrepare(IEEE8023_Source_RFC2464IsMulticast,debug_message,Server,OSI,IEEE8023);
        //RX(Server,OSI,RFC4291)(skb,shn,soi,(void*)(soi+1));
        return;
    }

    switch(soi->Type)
    {
        case cpu_to_be16(2048):
            DebugValuePrepare(IEEE8023_Source_RFC791,debug_message,Server,OSI,IEEE8023);
          //  RX(Server,OSI,RFC791)(skb,shn,soi,(void*)(soi+1));
            return;
        case cpu_to_be16(2054):
            DebugValuePrepare(IEEE8023_Source_RFC826,debug_message,Server,OSI,IEEE8023);
            //RX(Server,OSI,RFC826)(skb,shn,soi,(void*)(soi+1));
            return;
           // RX(Server,OSI,RFC826)(skb,shn,soi,(void*)(soi+1));
            return;
        default:
            DebugValuePrepare(IEEE8023_Source_RFC8200,debug_message,Server,OSI,IEEE8023);
            // RX(Server,OSI,RFC8200)(skb,shn,soi,(void*)(soi+1));
            return;
    }
}

FnTX(Server,OSI,IEEE8023,Type,Choice)(StructAlign(Server,OSI,IEEE8023)*soi)
{
    switch(soi->Type)
    {
        case cpu_to_be16(2048):
            //TX(Server,OSI,RFC791)(soi,(void*)(soi+1));
            break;
        case cpu_to_be16(34525):
            //TX(Server,OSI,RFC8200,Next,Choice)(soi,(void*)(soi+1));
            break;
    }
}

InitLibrary(Server,OSI,IEEE8023)
{
    DebugOutputPrepare(Server,OSI,IEEE8023);
    DebugMessagesPrepare(IEEE8023_Source_RFC2464IsMulticast,Server,OSI,IEEE8023);
    DebugMessagesPrepare(IEEE8023_Source_Broadcast,Server,OSI,IEEE8023);
    DebugMessagesPrepare(IEEE8023_Source_RFC791,Server,OSI,IEEE8023);
    DebugMessagesPrepare(IEEE8023_Source_RFC826,Server,OSI,IEEE8023);
    DebugMessagesPrepare(IEEE8023_Source_RFC8200,Server,OSI,IEEE8023);

}

QuitLibrary(Server,OSI,IEEE8023)
{
}
