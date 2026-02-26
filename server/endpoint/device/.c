#include ".h"
InitGlobalCache(endpoint_device)
Function(endpoint_device*,endpoint_device,init,hardware_network*hn,IEEE802_3MAC*mac){
    WaitForLock(hn->lock);
    endpoint_device*dev;
    list_for_each_entry(dev,&hn->endpoint_device,node)
        if(dev->address.path.normal.h==mac->path.normal.h&&dev->address.path.normal.l==mac->path.normal.l){
            dev->ref++;
            UpdateList(dev->node,hn->endpoint_device);
            ReleaseLock(hn->lock);
            return dev;
        }
    dev=CacheAlloc(endpoint_device);
    if(!dev){
        ReleaseLock(hn->lock);  
        return NULL;
    }
    dev->address.path.normal.h=mac->path.normal.h;
    dev->address.path.normal.l=mac->path.normal.l;
    dev->ref=1;
    dev->hn=hn;
    InitList(dev->node);
    InitList(dev->gateway);
    AddList(dev->node,hn->endpoint_device);
    ReleaseLock(hn->lock);
    return dev;
}
Void(endpoint_device,exit,endpoint_device*dev){
    hardware_network*hn=dev->hn;
    WaitForLock(hn->lock);
    if(!--dev->ref){
        list_del(&dev->node);
        CacheFree(endpoint_device,dev);
    }
    ReleaseLock(hn->lock);
}
Function(buffer,endpoint_device,CX,endpoint_device*dev,IEEE802_3**ieee802_3){
    buffer buff=Call(hardware_network,CX,dev->hn,ieee802_3);
    if(!buff)return NULL;
    (*ieee802_3)->dest.mac.path.normal.h=dev->address.path.normal.h;
    (*ieee802_3)->dest.mac.path.normal.l=dev->address.path.normal.l;
    return buff;
}