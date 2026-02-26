#include ".h"
#include "../endpoint/device/.h"
#include "../RFC791/.h"
#include "../RFC826/.h"
#include "../RFC8200/.h"
Void(IEEE802_3,RX,buffer buff,hardware_network*nh){
    IEEE802_3*ieee802_3=(IEEE802_3*)skb_mac_header(buff);
    endpoint_device*dev=Call(endpoint_device,init,nh,&ieee802_3->src.mac);
    if(!dev)return;
    switch(ieee802_3->type){
        case cpu_to_be16(2048):
            Call(RFC791,RX,buff,dev,ieee802_3,(RFC791*)(ieee802_3+1));
            break;
        case cpu_to_be16(2054):
            Call(RFC826,RX,buff,dev,ieee802_3,(RFC826*)(ieee802_3+1));
            break;
        case cpu_to_be16(34525):
            Call(RFC8200,RX,buff,dev,ieee802_3,(RFC8200*)(ieee802_3+1));
            break;
    }
    Call(endpoint_device,exit,dev);
}