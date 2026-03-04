#include ".h"
#include "../.h"
#include "../../EUI48/.h"
#include "../../overflow/.h"
DefineStructBody(networkDevice){
    EUI48 eui48;
    Network*network;
    Overflow overflow;
    StructListNode(node);
};
networkDevice*networkDeviceUse(Network*network,EUI48*src){

    struct list*n=GetNetworkDevicesByNetwork(network);

    return NULL;
}
void networkDeviceUnused(networkDevice*networkdevice){

}