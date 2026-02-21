#include ".h"
static int hardware_network_incoming(buffer skb,struct net_device*,struct packet_type*pt,struct net_device*){

    return NET_RX_DROP;
}
Void(hardware_network,init,void){
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
            InitList(hn->g);
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