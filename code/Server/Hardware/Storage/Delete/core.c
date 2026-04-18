#include ".setup"
#include "../Block/.setup"
#include "../Config/.setup"

Fn(void,Server,Hardware,Storage,Delete,Setup)(StructMemory(Server,Hardware,Storage)*shs)
{
    SyncLock(&shs->scs)return;
    StructDisk(Server,Hardware,Storage,Config)*shsc=(void*)shs->shsb->data;
    if(!shsc->deleted)
    {
        shs->shsb_=NULL;
        SyncUnlock(&shs->scs);
        return;
    }
    BlockSet(shs->shsb_,shs->id,shsc->deletepointer)
        shs->shsb_=NULL;
    SyncUnlock(&shs->scs);
}

Fn(void,Server,Hardware,Storage,Delete,Exit)(StructMemory(Server,Hardware,Storage)*shs)
{
    BlockRelease(shs->shsb_);
}

Fn(void,Server,Hardware,Storage,Delete,By,Pointer)(StructMemory(Server,Hardware,Storage,Block)*shsb)
{
    if(!shsb)return;
    SyncLock(&shsb->shs_storage->scs)return;
    if(!shsb->shs_storage->shsb_)
    {
        SyncUnlock(&shsb->shs_storage->scs);
        Call(Server,Hardware,Storage,Delete,Setup)(shsb->shs_storage);
        SyncLock(&shsb->shs_storage->scs)return;
    }
    memset(shsb->data,0,1024);
    BlockFlush(shsb);
    StructDisk(Server,Hardware,Storage,Config)*shsc=(void*)shsb->shs_storage->shsb->data;
    shsc->deleted++;
    if(shsc->deleted==1)
    {
        shsc->deleteindex=0;
        shsc->deletepointer=shsb->id;
        shsb->shs_storage->shsb_=shsb;
        SyncGet(&shsb->scs);
        BlockFlush(shsb->shs_storage->shsb);
        SyncUnlock(&shsb->shs_storage->scs);
        return;
    }
    StructDisk(Server,Hardware,Storage,Delete)*shsd=(void*)shsb->shs_storage->shsb_->data;
    shsd->ids[shsc->deleteindex]=shsb->id;
    if(++shsc->deleteindex==127)
    {
        StructDisk(Server,Hardware,Storage,Delete)*newshsd=(void*)shsb->data;
        newshsd->prev=shsc->deletepointer;
        shsc->deletepointer=shsb->id;
        shsc->deleteindex=0;
        BlockRelease(shsb->shs_storage->shsb_);
        shsb->shs_storage->shsb_=shsb;
        SyncGet(&shsb->scs);
    }
    BlockFlush(shsb->shs_storage->shsb_);
    BlockFlush(shsb->shs_storage->shsb);
    SyncUnlock(&shsb->shs_storage->scs);
}

Fn(StructMemory(Server,Hardware,Storage,Block)*,Server,Hardware,Storage,Delete,Use)(StructMemory(Server,Hardware,Storage)*shs)
{
    SyncLock(&shs->scs)return NULL;
    if(!shs->shsb_)
    {
        SyncUnlock(&shs->scs);
        Call(Server,Hardware,Storage,Delete,Setup)(shs);
        SyncLock(&shs->scs)return NULL;
    }
    StructDisk(Server,Hardware,Storage,Config)*shsc=(void*)shs->shsb->data;
    if(!shsc->deleted)
    {
        SyncUnlock(&shs->scs);
        return NULL;
    }
    if(shsc->deleted==1)
    {
        StructMemory(Server,Hardware,Storage,Block)*shsb=shs->shsb_;
        shsc->deleted=0;
        shs->shsb_=NULL;
        BlockFlush(shs->shsb);
        SyncUnlock(&shs->scs);
        return shsb;
    }
    StructDisk(Server,Hardware,Storage,Delete)*shsd=(void*)shs->shsb_->data;
    u64 blockid=shsd->ids[shsc->deleteindex];
    if(!shsc->deleteindex&&shsc->deleted>126)
    {
        u64 prev=shsd->prev;
        BlockRelease(shs->shsb_);
        BlockSet(shs->shsb_,shs->id,prev)shs->shsb_=NULL;
        shsc->deletepointer=prev;
        shsc->deleteindex=126;
    }
    if(shsc->deleteindex)shsc->deleteindex--;
    shsc->deleted--;
    BlockFlush(shs->shsb_);
    BlockFlush(shs->shsb);
    SyncUnlock(&shs->scs);
    BlockGet(shsb,shs->id,blockid)return NULL;
    memset(shsb->data,0,1024);
    BlockFlush(shsb);
    return shsb;
}

InitLibrary(Server,Hardware,Storage,Delete)
{
}

QuitLibrary(Server,Hardware,Storage,Delete)
{
}
