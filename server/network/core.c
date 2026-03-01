#include ".h"
#include "../EUI48/.h"

DefineStructBody(Network){
    struct packet_type pt;
    StructLock(lock);
    u8 ignore:4,valid:4;
    Overflow overflow;
    StructListNode(node,devices);
    netdev_features_t features_old,wanted_old,gso_partial_features_old;
};
EUI48*network_Get(Network*network){
    return (EUI48*)network->pt.dev->dev_addr;
}
InitGlobalMemory(Network);
DefineListHead(Networks);
GlobalLock(NetworkLock);
static bool network_use(Network*network)
{
    WaitForLock(network->lock);
    if(network->valid||!network->ignore){
        bool result=OverFlowIncrement(&network->overflow);
        ReleaseLock(network->lock);
        return result;
    }
    ReleaseLock(network->lock);
    return false;
}
static bool network_unused(Network*network)
{
    WaitForLock(network->lock);
    if(network->valid||!network->ignore){
        bool result=OverFlowDecrement(&network->overflow);
        ReleaseLock(network->lock);
        return result;
    }
    ReleaseLock(network->lock);
    return true;
}
struct sk_buff*network_PX(Network*network)
{
    struct sk_buff*buff=alloc_skb(2048,GFP_KERNEL);
    if(!buff){
        WaitForLock(network->lock);
            OverFlowBlock(&network->overflow);
        ReleaseLock(network->lock);
        return NULL;
    }
    if(!network_use(network)){
        kfree_skb(buff);
        WaitForLock(network->lock);
            OverFlowBlock(&network->overflow);
        ReleaseLock(network->lock);
        return NULL;
    }
    Set(buff,pkt_type,PACKET_OUTGOING);
    Set(buff,ip_summed,CHECKSUM_NONE);
    Set(buff,csum,0);
    Set(buff,dev,network->pt.dev);
    skb_reset_mac_header(buff);
    return buff;
}
void network_CX(Network*network,struct sk_buff*buff){
    if(buff)
        kfree_skb(buff);
    network_unused(network);
}
void network_TX(Network*network,struct sk_buff*buff)
{
    if(!buff||!IsOnline()){
        network_CX(network,buff);
        return;
    }
    dev_queue_xmit(buff);
    network_unused(network);
}

DefineStructHeadBody(RX){
    struct sk_buff*skb;
    Network*network;
    struct work_struct ws;
};
InitGlobalMemory(RX);
static void network_RX(struct work_struct*ws)
{
    RX*rx=container_of(ws,RX,ws);
    if(IsOnline())
        IEEE802_3_RX(rx->skb,rx->network,(u8*)skb_mac_header(rx->skb));
    network_unused(rx->network);
    kfree_skb(rx->skb);
    MemoryFree(RX,rx);
}
static int network_IRQRX(struct sk_buff*skb,struct net_device*dev,struct packet_type*pt,struct net_device*orig_dev){
    if(!skb||Get(skb,pkt_type)==PACKET_OUTGOING||!IsOnline()||!Get(skb,dev)||Get(skb,len)<34||!pskb_may_pull(skb,Get(skb,len)))
        return NET_RX_SUCCESS;
    Network*network=container_of(pt,Network,pt);
    WaitForLock(network->lock);
    if(Get(network,ignore)){
        ReleaseLock(network->lock);
        return NET_RX_DROP;
    }
    ReleaseLock(network->lock);
    if(!network_use(network))
        return NET_RX_DROP;
    RX*rx=MemoryAlloc(RX);
    if(!rx){
        WaitForLock(network->lock);
        OverFlowBlock(&network->overflow);
        ReleaseLock(network->lock);
        network_unused(network);
        return NET_RX_DROP;
    }
    rx->skb=skb_get(skb);
    rx->network=network;
    INIT_WORK(&rx->ws,network_RX);
    queue_work(system_wq,&rx->ws);
    return NET_RX_DROP;
}
void network_DeviceAdd(Network*network,networkDevice*networkdevice,u8 version)
{
    networkdevice->version=version;
    OverFlowInit(&network->overflow);
    networkdevice->network=network;
    InitList(networkdevice->node);
    AddTop(networkdevice->node,network->devices);
}
void network_DeviceRemove(Network*network,networkDevice*networkdevice)
{
    DelList(networkdevice->node);
}
networkDevice*network_DeviceGet(Network*network,u8 version,void*address)
{
    networkDevice*dev,*tmp;
    ForEachEntrySafe(dev,tmp,network->devices,node)
        if(dev->version==version&&(dev->version==4?network_Device_V4Validate(dev,(__be32*)address):network_Device_V6Validate(dev,(u128*)address))){
            UpdateList(dev->node,network->devices);
            return dev;
        }
    return NULL;
}
void network_init(void)
{
    InitList(Networks);
    InitLock(NetworkLock);
    InitMemory(RX);
    InitMemory(Network);
    struct net_device*dev;
    for_each_netdev(&init_net,dev){
        Network*network=MemoryAlloc(Network);
        if(!network)
            continue;
        InitLockMany(network->lock);
        InitListMany(network->node,network->devices);
        OverFlowInit(&network->overflow);
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
            Set(network,pt.func,network_IRQRX);
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
    ExitMemory(RX);
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