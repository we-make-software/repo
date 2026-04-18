#include ".setup"
#include "../Block/.setup"

Fn(void,Server,Hardware,Storage,Config,Init)(StructMemory(Server,Hardware,Storage)*shs)
{
    BlockSet(shs->shsb,shs->id,0)return;
    StructDisk(Server,Hardware,Storage,Config)*shsc=(void*)shs->shsb->data;
    if(!memcmp(shsc->magic,"PCCPRC",6))return;
    memset(shs->shsb->data,0,1024);
    memcpy(shsc->magic,"PCCPRC",6);
    shsc->versionstorage=VERSION_STORAGE;
    shsc->used=1;
    BlockFlush(shs->shsb);
}

Fn(void,Server,Hardware,Storage,Config,Quit)(StructMemory(Server,Hardware,Storage)*shs)
{
    BlockRelease(shs->shsb);
}

Fn(u64,Server,Hardware,Storage,Config,Get,Used)(StructMemory(Server,Hardware,Storage)*shs)
{
    SyncLock(&shs->scs)return 0;
    StructDisk(Server,Hardware,Storage,Config)*shsc=(void*)shs->shsb->data;
    u64 used=shsc->used;
    SyncUnlock(&shs->scs);
    return used;
}

Fn(u64,Server,Hardware,Storage,Config,Get,Deleted)(StructMemory(Server,Hardware,Storage)*shs)
{
    SyncLock(&shs->scs)return 0;
    StructDisk(Server,Hardware,Storage,Config)*shsc=(void*)shs->shsb->data;
    u64 deleted=shsc->deleted;
    SyncUnlock(&shs->scs);
    return deleted;
}

InitLibrary(Server,Hardware,Storage,Config)
{
}

QuitLibrary(Server,Hardware,Storage,Config)
{
}
