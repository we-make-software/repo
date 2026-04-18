#include ".setup"
#include "../../Memory/.setup"
#include "../Delete/.setup"
#include "../Config/.setup"

static void Before(StructHelp(Server,Common,Sync)*scs)
{
    StructMemory(Server,Hardware,Storage,Block)*shsb=container_of(scs,StructMemory(Server,Hardware,Storage,Block),scs);
    SyncLock(&shsb->shs_storage->scs)return;
    list_del(&shsb->node);
    SyncUnlock(&shsb->shs_storage->scs);
}

static void After(StructHelp(Server,Common,Sync)*scs)
{
    StructMemory(Server,Hardware,Storage,Block)*shsb=container_of(scs,StructMemory(Server,Hardware,Storage,Block),scs);
    StorageRelease(shsb->shs_storage);
    MemoryDelete(shsb);
}

InitLibrary(Server,Hardware,Storage,Block)
{
    MemoryPrepare(Server,Hardware,Storage,Block);
}

QuitLibrary(Server,Hardware,Storage,Block)
{
}

Fn(StructMemory(Server,Hardware,Storage,Block)*,Server,Hardware,Storage,Block,Get)(u16 storageid,u64 blockid)
{
    StorageGet(shs,storageid)return NULL;
    SyncLock(&shs->scs)
    {
        StorageRelease(shs);
        return NULL;
    }
    {
        StructMemory(Server,Hardware,Storage,Block)*shsb;
        list_for_each_entry(shsb,&shs->block_node,node)
            if(shsb->id==blockid)
                if(SyncGet(&shsb->scs))
                {
                    if(shs->block_node.next!=&shsb->node)list_move(&shsb->node,&shs->block_node);
                    SyncUnlock(&shs->scs);
                    return shsb;
                }
    }
    MemoryGet(shsb,Server,Hardware,Storage,Block)
    {
        SyncUnlock(&shs->scs);
        StorageRelease(shs);
        return NULL;
    }
    shsb->id=blockid;
    shsb->shs_storage=shs;
    SyncSetup(&shsb->scs,Before,After);
    SyncGet(&shsb->scs);
    loff_t pos=(loff_t)blockid*1024;
    if(kernel_read(shs->file,shsb->data,1024,&pos)!=1024)
    {
        MemoryDelete(shsb);
        SyncUnlock(&shs->scs);
        StorageRelease(shs);
        return NULL;
    }
    list_add(&shsb->node,&shs->block_node);
    SyncUnlock(&shs->scs);
    return shsb;
}

Fn(StructMemory(Server,Hardware,Storage,Block)*,Server,Hardware,Storage,Block,New)(u16 storageid)
{
    StorageGet(shs,storageid)return NULL;
    StructMemory(Server,Hardware,Storage,Block)*shsb=Call(Server,Hardware,Storage,Delete,Use)(shs);
    if(shsb)return shsb;
    SyncLock(&shs->scs)
    {
        StorageRelease(shs);
        return NULL;
    }
    StructDisk(Server,Hardware,Storage,Config)*shsc=(void*)shs->shsb->data;
    if(shsc->used>=shs->size)
    {
        SyncUnlock(&shs->scs);
        StorageRelease(shs);
        return NULL;
    }
    u64 blockid=shsc->used++;
    BlockFlush(shs->shsb);
    SyncUnlock(&shs->scs);
    BlockGet(newblock,storageid,blockid)
    {
        StorageRelease(shs);
        return NULL;
    }
    memset(newblock->data,0,1024);
    BlockFlush(newblock);
    return newblock;
}

Fn(bool,Server,Hardware,Storage,Block,Lock)(StructMemory(Server,Hardware,Storage,Block)*shsb)
{
    if(!shsb)return false;
    SyncLock(&shsb->scs)return false;
    return true;
}

Fn(void,Server,Hardware,Storage,Block,Unlock)(StructMemory(Server,Hardware,Storage,Block)*shsb)
{
    if(shsb)SyncUnlock(&shsb->scs);
}

Fn(void,Server,Hardware,Storage,Block,Flush)(StructMemory(Server,Hardware,Storage,Block)*shsb)
{
    if(!shsb)return;
    if(!++shsb->flush)
    {
        loff_t pos=(loff_t)shsb->id*1024;
        kernel_write(shsb->shs_storage->file,shsb->data,1024,&pos);
    }
}

Fn(void,Server,Hardware,Storage,Block,Release)(StructMemory(Server,Hardware,Storage,Block)*shsb)
{
    if(shsb)SyncDelete(&shsb->scs);
}
