#ifndef endpoint_device_h
    #define endpoint_device_h
    #include "../IEEE802_3/.h"
    DefineStruct(endpoint_device){
        IEEE802_3MAC address;
        u128 ref;
        Lock lock;
        hardware_network*ns;
        list node
    };
    DefineFunction(endpoint_device*,endpoint_device,init,hardware_network*,IEEE802_3MAC*);
    DefineVoid(endpoint_device,exit,endpoint_device*);
#endif