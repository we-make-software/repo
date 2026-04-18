#include ".setup"

Fn(void,Server,Common,Sync,Init)(StructHelp(Server,Common,Sync)*scs,void(*before)(StructHelp(Server,Common,Sync)*),void(*after)(StructHelp(Server,Common,Sync)*))
{
    scs->ref=0;
    mutex_init(&scs->lock);
    scs->status=EnumValueHelp(Server,Common,Sync,Status,Active);
    scs->before=before;
    scs->after=after;
}

Fn(bool,Server,Common,Sync,Get)(StructHelp(Server,Common,Sync)*scs)
{
    SyncLock(scs)return false;
    if(scs->status==EnumValueHelp(Server,Common,Sync,Status,ProcessDeleting))
        scs->status=EnumValueHelp(Server,Common,Sync,Status,Active);
    scs->ref++;
    SyncUnlock(scs);
    return true;
}

Fn(bool,Server,Common,Sync,Lock)(StructHelp(Server,Common,Sync)*scs)
{
    mutex_lock(&scs->lock);
    if(scs->status==EnumValueHelp(Server,Common,Sync,Status,Delete))
    {
        mutex_unlock(&scs->lock);
        return false;
    }
    return true;
}

Fn(void,Server,Common,Sync,Unlock)(StructHelp(Server,Common,Sync)*scs)
{
    mutex_unlock(&scs->lock);
}

Fn(void,Server,Common,Sync,Delete)(StructHelp(Server,Common,Sync)*scs)
{
    SyncLock(scs)return;
    if(!--scs->ref)
    {
        scs->status=EnumValueHelp(Server,Common,Sync,Status,ProcessDeleting);
        SyncUnlock(scs);
        SyncLock(scs)return;
        if(scs->status==EnumValueHelp(Server,Common,Sync,Status,Active))
        {
            SyncUnlock(scs);
            return;
        }
        scs->status=EnumValueHelp(Server,Common,Sync,Status,Delete);
        SyncUnlock(scs);
        if(scs->before)scs->before(scs);
        if(scs->after)scs->after(scs);
        return;
    }
    SyncUnlock(scs);
}

InitLibrary(Server,Common,Sync)
{
}

QuitLibrary(Server,Common,Sync)
{
}
