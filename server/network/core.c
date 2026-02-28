#include ".h"
//i place this bce it can be a security risk if some other file includes this and it can cause circular dependencies
DefineStructBody(Network){
    struct packet_type pt;
    StructLock(lock);
    u8 ignore:4,valid:4;
    u128 ref;
    u8 index;
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

            Set(network,features_old,dev->features);
            Set(network,wanted_old,dev->wanted_features);
            Set(network,gso_partial_features_old,dev->gso_partial_features);

            Clear(dev,features,
                NETIF_F_GRO|NETIF_F_GRO_HW|NETIF_F_LRO|
                NETIF_F_GSO_MASK|NETIF_F_ALL_TSO|NETIF_F_GSO_PARTIAL|
                NETIF_F_SG|
                NETIF_F_RXCSUM|NETIF_F_CSUM_MASK|
                NETIF_F_HW_VLAN_CTAG_RX|NETIF_F_HW_VLAN_CTAG_TX|NETIF_F_HW_VLAN_CTAG_FILTER|
                NETIF_F_HW_VLAN_STAG_RX|NETIF_F_HW_VLAN_STAG_TX|NETIF_F_HW_VLAN_STAG_FILTER
            );
            Clear(dev,wanted_features,
                NETIF_F_GRO|NETIF_F_GRO_HW|NETIF_F_LRO|
                NETIF_F_GSO_MASK|NETIF_F_ALL_TSO|NETIF_F_GSO_PARTIAL|
                NETIF_F_SG|
                NETIF_F_RXCSUM|NETIF_F_CSUM_MASK|
                NETIF_F_HW_VLAN_CTAG_RX|NETIF_F_HW_VLAN_CTAG_TX|NETIF_F_HW_VLAN_CTAG_FILTER|
                NETIF_F_HW_VLAN_STAG_RX|NETIF_F_HW_VLAN_STAG_TX|NETIF_F_HW_VLAN_STAG_FILTER
            );
            Set(dev,gso_partial_features,0);
            netdev_update_features(dev);
            Set(network,pt.dev,dev);
            //Set(network,pt.func,PTF);
            //this its call the overflow system
            Set(network,pt.type,htons(ETH_P_ALL));
            dev_add_pack(&network->pt);
            rtnl_unlock();
    }
    synchronize_net();
}
void network_exit(void)
{
    Network*network,*tmp;
    ForEachEntrySafe(network,tmp,Networks,node){
        //here wee need to call a function that call each device inside the device list to kill or do a action for that.
        rtnl_lock();
        dev_remove_pack(&network->pt);
        Set(network->pt.dev,features,network->features_old);
        Set(network->pt.dev,wanted_features,network->wanted_old);
        Set(network->pt.dev,gso_partial_features,network->gso_partial_features_old);
        netdev_update_features(network->pt.dev);
        rtnl_unlock();
        list_del(&network->node);
        MemoryFree(Network,network);
    }
    synchronize_net();
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