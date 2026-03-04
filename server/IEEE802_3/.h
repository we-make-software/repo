#pragma once
#include "../network/.h"
#include "../EUI48/.h"
#include "../network/device/.h"
DefineNetworkStructHeadBody(IEEE802_3){
    union{
        EUI48 eui48;
        __be16 h;
    }dest,src;
    __be16 type;
};
networkDevice*networkDeviceUse(Network*network,EUI48*src);
void networkDeviceUnused(networkDevice*networkdevice);