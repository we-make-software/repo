#include ".h"
Void(IEEE802_3,RX,buffer buff,hardware_network*nh){
    IEEE802_3*ieee802_3=(IEEE802_3*)buff->data;
    endpoint_device*dev=Call(endpoint_device,init,nh,ieee802_3->src);
    if(!dev)return;

    Call(endpoint_device,exit,dev);
}