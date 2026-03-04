#include ".h"

void IEEE802_3_RX(struct sk_buff*skb,Network*network,u8*buff){
    IEEE802_3*ieee802_3=(IEEE802_3*)buff;
    networkDevice*networkdevice=networkDeviceUse(network,&ieee802_3->src);    
    if(!networkdevice)
        return NULL;
    switch(ieee802_3->type){
        case cpu_to_be16(2048):
        
        break;
        case cpu_to_be16(2054):
        break;
        case cpu_to_be16(34525):
        break;
    }    
    networkDeviceUnused(networkdevice);
}
