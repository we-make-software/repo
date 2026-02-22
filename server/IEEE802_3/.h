#ifndef IEEE802_3_h
    #define IEEE802_3_h
    #include "../server.h"
    DefineNetworkStruct(IEEE802_3MAC){
        union{
            NetworkAddStruct{
                u8 ig:1,ul:1,res:6;
            }infomation;
            NetworkAddStruct{
                __be32 h;
                __be16 l;
            } normal;
        }path;
    };
    DefineNetworkStruct(IEEE802_3){
        union{
            IEEE802_3MAC mac;
            __be16 h;
        }dest,src;
        __be16 type;
    };
#endif