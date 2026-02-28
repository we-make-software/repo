#include ".h"
//i place this bce it can be a security risk if some other file includes this and it can cause circular dependencies
DefineStructBody(Network){
    struct packet_type pt;
    StructLock(lock);
    u8 ignore:4,valid:4;
    StructListNode(node,devices);
    netdev_features_t
                features_old,
                wanted_old,
                gso_partial_features_old;
};
InitGlobalMemory(Network);
DefineListHead(Networks);
GlobalLock(NetworkLock);
void network_init(void)
{
    InitList(Networks);
    InitLock(NetworkLock);
    InitMemory(Network);
    struct net_device*dev;
    for_each_netdev(&init_net,dev){
        Network*network=MemoryAlloc(Network);
        if(!network)
            continue;
        InitLockMany(network->lock);
        InitListMany(network->node,network->devices);
        AddTop(network->node,Networks);
        rtnl_lock();
    
    }
    /* din network init kode */
}
void network_exit(void)
{
    ExitMemory(Network);
    /* din network exit kode */
}
struct packet_type*GetPacketTypeByNetwork(Network*network)
{
    return &network->pt;
}
struct list_head*GetNetworkDevicesByNetwork(Network*network)
{
    return &network->devices;
}