#include ".h"
InitGlobalCache(endpoint_device)
Function(endpoint_device*,endpoint_device,init,hardware_network*nh,IEEE802_3MAC*mac){
    WaitForLock(nh->lock);
    endpoint_device*dev;
    list_for_each_entry(dev,&nh->endpoint_device,node)
        if(dev->address.path.normal.h==mac->path.normal.h&&dev->address.path.normal.l==mac->path.normal.l){
            dev->ref++;
            UpdateList(dev->node,nh->endpoint_device);
            ReleaseLock(nh->lock);
            return dev;
        }
    dev=CacheAlloc(endpoint_device);
    if(!dev){
        ReleaseLock(nh->lock);  
        return NULL;
    }
    dev->address.path.normal.h=mac->path.normal.h;
    dev->address.path.normal.l=mac->path.normal.l;
    dev->ref=1;
    dev->ns=nh;
    InitList(dev->node);
    AddList(dev->node,nh->endpoint_device);
    ReleaseLock(nh->lock);
    return dev;
}
Void(endpoint_device,exit,endpoint_device*dev){
    hardware_network*nh=dev->ns;
    WaitForLock(nh->lock);
    if(!--dev->ref){
        list_del(&dev->node);
        CacheFree(endpoint_device,dev);
    }
    ReleaseLock(nh->lock);
}