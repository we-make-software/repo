#include ".h"
static struct workqueue_struct*hardware_network_work_rx_WQ;
static LIST_HEAD(hardware_network_works);
static DEFINE_SPINLOCK(hardware_network_work_rxlock);
DefineStruct(hardware_network_work_rx){
    struct work_struct
        ws;
    buffer
        buff;
    hardware_network
        *nh;
    list
        node;
};
InitGlobalCache(hardware_network_work_rx)
static void hardware_network_work_rx_ws(struct work_struct*ws){
    hardware_network_work_rx*w=container_of(ws,hardware_network_work_rx,ws);
    Call(IEEE802_3,RX,w->buff,w->nh);
    spin_lock_bh(&hardware_network_work_rxlock);
    list_del(&w->node);
    spin_unlock_bh(&hardware_network_work_rxlock);
    if(w->buff)
        kfree_skb(w->buff);
    CacheFree(hardware_network_work_rx,w);
}
Void(hardware_network,work_rx_creator,buffer buff,hardware_network*nh){
    hardware_network_work_rx*w=CacheAlloc(hardware_network_work_rx);
    if(!w)
        return;
    INIT_WORK(&w->ws,hardware_network_work_rx_ws);
    InitList(w->node);
    Set(w,nh,nh);
    Set(w,buff,skb_get(buff));
    spin_lock_bh(&hardware_network_work_rxlock);
    AddList(w->node,hardware_network_works);
    spin_unlock_bh(&hardware_network_work_rxlock);
    queue_work(hardware_network_work_rx_WQ,&w->ws);
}
static int hardware_network_incoming(buffer skb,struct net_device*,struct packet_type*pt,struct net_device*){
    if(!skb||skb->pkt_type==PACKET_OUTGOING||!OnlineStatus||!skb->dev||skb->len<34||!pskb_may_pull(skb,skb->len))
        return NET_RX_SUCCESS;
    Call(hardware_network,work_rx_creator,skb,container_of(pt,hardware_network,pt));
    return NET_RX_DROP;
}
Void(hardware_network,init,void){
        InitCache(hardware_network_work_rx);
        hardware_network_work_rx_WQ=alloc_workqueue("hardware_network_work_rx_WQ",WQ_UNBOUND,0);
        struct net_device*dev;
        for_each_netdev(&init_net,dev){
            if((dev->flags&IFF_LOOPBACK)||({struct device*dp=dev->dev.parent;is_zero_ether_addr(dev->dev_addr)||(dp&&dp->bus&&!strcmp(dp->bus->name,"usb"));}))
                continue;
            hardware_network*hn=kmalloc(sizeof(*hn),GFP_KERNEL);
            if(!hn)
                continue;

            InitLock(hn->lock);
            InitList(hn->node);
            AddList(hn->node,hardware_networks);
            InitList(hn->endpoint_device);
            rtnl_lock();
            Set(hn,features_old,dev->features);
            Set(hn,wanted_old,dev->wanted_features);
            Set(hn,gso_partial_features_old,dev->gso_partial_features);

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
            Set(hn,pt.dev,dev);
            Set(hn,pt.func,hardware_network_incoming);
           Set(hn,pt.type,htons(ETH_P_ALL));
            dev_add_pack(&hn->pt);
            rtnl_unlock();
        }
        synchronize_net();
}
Void(hardware_network,exit,void){
    {
        hardware_network_work_rx*w,*tmp;
        flush_workqueue(hardware_network_work_rx_WQ);
        destroy_workqueue(hardware_network_work_rx_WQ);
        spin_lock_bh(&hardware_network_work_rxlock);
        list_for_each_entry_safe(w,tmp,&hardware_network_works,node){
            list_del(&w->node);
            spin_unlock_bh(&hardware_network_work_rxlock);
            if(w->buff)
                kfree_skb(w->buff);
            CacheFree(hardware_network_work_rx,w);
            spin_lock_bh(&hardware_network_work_rxlock);
        }
        spin_unlock_bh(&hardware_network_work_rxlock);
        ExitCache(hardware_network_work_rx);
    }
    {
        hardware_network*hn,*tmp;
        list_for_each_entry_safe(hn,tmp,&hardware_networks,node){
            rtnl_lock();
            dev_remove_pack(&hn->pt);
            Set(hn->pt.dev,features,hn->features_old);
            Set(hn->pt.dev,wanted_features,hn->wanted_old);
            Set(hn->pt.dev,gso_partial_features,hn->gso_partial_features_old);
            netdev_update_features(hn->pt.dev);
            rtnl_unlock();
            list_del(&hn->node);
            kfree(hn);
        }
        synchronize_net();
    }
}
Function(buffer,hardware_network,CX,hardware_network*hn,IEEE802_3**ieee802_3){
    buffer buff=alloc_skb(2048,GFP_KERNEL);
    if(!buff)return NULL;
    Set(buff,pkt_type,PACKET_OUTGOING);
    Set(buff,ip_summed,CHECKSUM_NONE);
    Set(buff,csum,0);
    Set(buff,dev,hn->pt.dev);
    skb_reset_mac_header(buff);
    *ieee802_3=skb_put(buff,sizeof(IEEE802_3));
    memcpy(&(*ieee802_3)->src.mac,hn->pt.dev->dev_addr,6);
    return buff;
}
Void(hardware_network,TX,buffer buff,IEEE802_3*ieee802_3){
    Call(IEEE802_3,TX,ieee802_3);
    dev_queue_xmit(buff);
}