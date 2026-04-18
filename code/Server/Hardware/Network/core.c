#include ".setup"
#include "../../OSI/IEEE8023/.setup"
#ifndef MODULE
StructMemory(Server,Hardware,Network,Receive,Relay){
    StructMemory(Server,Hardware,Network)*shn;
    struct sk_buff*skb;
    struct work_struct thread;
};
#endif

static struct workqueue_struct*queue;
static LIST_HEAD(nodes);

static void After(StructHelp(Server,Common,Sync)*scs)
{
    StructMemory(Server,Hardware,Network)*shn=container_of(scs,StructMemory(Server,Hardware,Network),scs);
    MemoryDelete(shn);
}

Fn(void,Server,Hardware,Network,Release)(StructMemory(Server,Hardware,Network)*shn)
{
    if(shn)SyncDelete(&shn->scs);
}

Fn(StructMemory(Server,Hardware,Network)*,Server,Hardware,Network,Get,Or,Null)(u16 id)
{
    StructMemory(Server,Hardware,Network)*shn;
    list_for_each_entry(shn,&nodes,node)
        if(shn->id==id)
            return SyncGet(&shn->scs)?shn:NULL;
    return NULL;
}

Fn(bool,Server,Hardware,Network,Use)(StructMemory(Server,Hardware,Network)*shn)
{
    if(!shn)return false;
    return SyncGet(&shn->scs);
}

static void Work(struct work_struct*thread)
{
    StructMemory(Server,Hardware,Network,Receive)*shnr=container_of(thread,StructMemory(Server,Hardware,Network,Receive),thread);
    if(!IsRunning())
    {
        dev_kfree_skb_any(shnr->skb);
        MemoryDelete(shnr);
        return;
    }
    if(NetworkAdapterUse(shnr->shn))
    {
        RX(Server,OSI,IEEE8023)(shnr->skb,shnr->shn,(void*)skb_mac_header(shnr->skb));
        NetworkAdapterRelease(shnr->shn);
    }
    dev_kfree_skb_any(shnr->skb);
    MemoryDelete(shnr);
}

static void QueueReceive(StructMemory(Server,Hardware,Network)*shn,struct sk_buff*skb)
{
    MemoryGet(shnr,Server,Hardware,Network,Receive)
    {
        dev_kfree_skb_any(skb);
        return;
    }
    shnr->shn=shn;
    shnr->skb=skb;
    INIT_WORK(&shnr->thread,Work);
    queue_work(queue,&shnr->thread);
}

#ifndef MODULE
static void Relay(struct work_struct*thread)
{
    StructMemory(Server,Hardware,Network,Receive,Relay)*shnrr=container_of(thread,StructMemory(Server,Hardware,Network,Receive,Relay),thread);
    if(!IsRunning())
    {
        dev_kfree_skb_any(shnrr->skb);
        kfree(shnrr);
        return;
    }
    QueueReceive(shnrr->shn,shnrr->skb);
    kfree(shnrr);
}

static rx_handler_result_t Receive(struct sk_buff **pskb)
{
    struct sk_buff*skb=*pskb;
    if(!skb||!IsRunning()||skb->pkt_type==PACKET_OUTGOING||!skb->dev||(skb->dev->flags&IFF_LOOPBACK)||skb->len<34||!pskb_may_pull(skb,skb->len))
        return RX_HANDLER_PASS;
    
    StructMemory(Server,Hardware,Network)*shn=(StructMemory(Server,Hardware,Network)*)rcu_dereference(skb->dev->rx_handler_data);
    if(!shn)
        return RX_HANDLER_PASS;

    StructMemory(Server,Hardware,Network,Receive,Relay)*shnrr=kmalloc(sizeof(*shnrr),GFP_ATOMIC);
    if(!shnrr)
    {
        dev_kfree_skb_any(skb);
        return RX_HANDLER_CONSUMED;
    }
    shnrr->shn=shn;
    shnrr->skb=skb;
    INIT_WORK(&shnrr->thread,Relay);
    schedule_work(&shnrr->thread);
    return RX_HANDLER_CONSUMED;
}
#else
static int Receive(struct sk_buff*skb,struct net_device*dev,struct packet_type*pt,struct net_device*orig_dev)
{
    if(!skb||!IsRunning()||skb->pkt_type==PACKET_OUTGOING||!skb->dev||(skb->dev->flags&IFF_LOOPBACK)||skb->len<34||!pskb_may_pull(skb,skb->len))
        return NET_RX_SUCCESS;
    
    QueueReceive(container_of(pt,StructMemory(Server,Hardware,Network),pt),skb_get(skb));
    return NET_RX_DROP;
}
#endif

FnNew(Server,Hardware,Network)(StructMemory(Server,Hardware,Network)*shn)
{
    struct sk_buff*buff=alloc_skb(2048,GFP_KERNEL);
    if(!buff)
        return NULL;
    buff->pkt_type=PACKET_OUTGOING;
    buff->ip_summed=CHECKSUM_NONE;
    buff->csum=0;
    buff->dev=shn->dev;
    skb_reset_mac_header(buff);
    return buff;
}

static FnTX(Server,Hardware,Network)(struct sk_buff*skb,bool debug)
{
    TX(Server,OSI,IEEE8023,Type,Choice)((void*)skb_mac_header(skb));
    if(debug)
    {
        dev_kfree_skb_any(skb);
        return;
    }
    dev_queue_xmit(skb);
}

Fn(void,Linux,SKB,Free)(struct sk_buff**skb)
{
    if(skb&&*skb)
    {
        dev_kfree_skb_any(*skb);
        *skb=NULL;
    }
}

Fn(void,Linux,SKB,Send)(struct sk_buff*skb,bool debug)
{
    TX(Server,Hardware,Network)(skb,debug);
}

InitLibrary(Server,Hardware,Network)
{
    u16 id=0;
    MemoryPrepare(Server,Hardware,Network);
    MemoryPrepare(Server,Hardware,Network,Receive);
    queue=alloc_workqueue("hardware,network,receive",WQ_HIGHPRI,0);

    struct net_device*dev;
    rtnl_lock();
    for_each_netdev(&init_net,dev)
    {
        if((dev->flags&IFF_LOOPBACK)||is_zero_ether_addr(dev->dev_addr))
            continue;
        
        MemoryGet(shn,Server,Hardware,Network)continue;
        shn->id=id++;
        shn->dev=dev;
        SyncSetup(&shn->scs,NULL,After);
        SyncGet(&shn->scs);
        list_add_tail(&shn->node,&nodes);
        INIT_LIST_HEAD(&shn->shnd_node);

        shn->features_old=dev->features;
        shn->wanted_features_old=dev->wanted_features;
        shn->gso_partial_features_old=dev->gso_partial_features;

#ifdef MODULE
        shn->pt.dev=dev;
        shn->pt.type=htons(ETH_P_ALL);
        shn->pt.func=Receive;
        dev_add_pack(&shn->pt);
#else
        netdev_rx_handler_register(dev,Receive,shn);
#endif

        dev->features&=~(NETIF_F_GRO|NETIF_F_GRO_HW|NETIF_F_LRO|NETIF_F_GSO_MASK|NETIF_F_SG|NETIF_F_RXCSUM|NETIF_F_CSUM_MASK|NETIF_F_HW_VLAN_CTAG_RX|NETIF_F_HW_VLAN_CTAG_TX|NETIF_F_HW_VLAN_CTAG_FILTER|NETIF_F_HW_VLAN_STAG_RX|NETIF_F_HW_VLAN_STAG_TX|NETIF_F_HW_VLAN_STAG_FILTER);
        dev->wanted_features&=~(NETIF_F_GRO|NETIF_F_GRO_HW|NETIF_F_LRO|NETIF_F_GSO_MASK|NETIF_F_SG|NETIF_F_RXCSUM|NETIF_F_CSUM_MASK|NETIF_F_HW_VLAN_CTAG_RX|NETIF_F_HW_VLAN_CTAG_TX|NETIF_F_HW_VLAN_CTAG_FILTER|NETIF_F_HW_VLAN_STAG_RX|NETIF_F_HW_VLAN_STAG_TX|NETIF_F_HW_VLAN_STAG_FILTER);
        dev->gso_partial_features=0;
        netdev_update_features(dev);
    }
    rtnl_unlock();
    synchronize_net();
}

QuitLibrary(Server,Hardware,Network)
{
    rtnl_lock();
    StructMemory(Server,Hardware,Network)*shn,*tmp;
    list_for_each_entry_safe(shn,tmp,&nodes,node)
    {
        shn->dev->features=shn->features_old;
        shn->dev->wanted_features=shn->wanted_features_old;
        shn->dev->gso_partial_features=shn->gso_partial_features_old;
        netdev_update_features(shn->dev);

#ifdef MODULE
        dev_remove_pack(&shn->pt);
#else
        netdev_rx_handler_unregister(shn->dev);
#endif
        list_del(&shn->node);
        NetworkAdapterRelease(shn);
    }
    rtnl_unlock();
    synchronize_net();
    
    if(queue)
    {
        flush_workqueue(queue);
        destroy_workqueue(queue);
    }
}
