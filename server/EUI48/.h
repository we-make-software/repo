#pragma once
#include "../core/.h"
DefineNetworkStructHeadBody(EUI48){
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
typedef enum{
    EUI48_IG_UNICAST=0,
    EUI48_IG_MULTICAST=1
}EUI48_IG;

typedef enum{
    EUI48_UL_GLOBAL=0,
    EUI48_UL_LOCAL=1
}EUI48_UL;

EUI48_IG EUI48_GetIG(EUI48*eui48);
EUI48_UL EUI48_GetUL(EUI48*eui48);
void EUI48_Clone(EUI48*from,EUI48*to);