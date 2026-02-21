#ifndef hardware_network_h
    #define hardware_network_h
    #include "../server.h"
    DefineStruct(hardware_network){
        netdev_features_t
            features_old,
            wanted_old,
            gso_partial_features_old;
    };
    DefineVoid(hardware_network,init,void);
    DefineVoid(hardware_network,exit,void);
#endif