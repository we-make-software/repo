#ifndef RFC791_h
    #define RFC791_h
    #include "../server.h"
    #include "../endpoint/device/.h"
    DefineStruct(RFC791){

    };
    DefineVoid(RFC791,RX,buffer,endpoint_device*,IEEE802_3*,RFC791*);
#endif