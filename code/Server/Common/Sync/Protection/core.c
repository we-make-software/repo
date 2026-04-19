#include ".setup"

static LIST_HEAD(nodes);
static DEFINE_MUTEX(lock);

static void Work(struct work_struct*thread)
{
    StructHelp(Server,Common,Sync,Protection)*scsp=container_of(to_delayed_work(thread),StructHelp(Server,Common,Sync,Protection),work);
    SyncLock(&scsp->scs){SyncDelete(&scsp->scs);return;}
    if(scsp->status!=EnumValueHelp(Server,Common,Sync,Protection,Status,Active))
    {
        SyncUnlock(&scsp->scs);
        SyncDelete(&scsp->scs);
        return;
    }

    struct timespec64 now;
    ktime_get_real_ts64(&now);
    if(timespec64_compare(&scsp->utc,&now)>0)
    {
        s64 diff=timespec64_to_jiffies(&scsp->utc)-timespec64_to_jiffies(&now);
        mod_delayed_work(system_wq,&scsp->work,diff);
        SyncUnlock(&scsp->scs);
        return;
    }

    scsp->status=EnumValueHelp(Server,Common,Sync,Protection,Status,Before);
    scsp->limition=0;
    mutex_lock(&lock);
    if(scsp->listed)
    {
        list_del_init(&scsp->node);
        scsp->listed=false;
    }
    mutex_unlock(&lock);
    SyncUnlock(&scsp->scs);

    if(scsp->handler)scsp->handler(scsp);

    SyncLock(&scsp->scs){SyncDelete(&scsp->scs);return;}
    scsp->status=EnumValueHelp(Server,Common,Sync,Protection,Status,After);
    SyncUnlock(&scsp->scs);
    SyncDelete(&scsp->scs);
}

Fn(void,Server,Common,Sync,Protection,Setup)(StructHelp(Server,Common,Sync,Protection)*scsp,u32 limition,struct timespec64 utc,void(*sync_before)(StructHelp(Server,Common,Sync)*),void(*sync_after)(StructHelp(Server,Common,Sync)*),void(*handler)(StructHelp(Server,Common,Sync,Protection)*),bool immediate)
{
    SyncSetup(&scsp->scs,sync_before,sync_after);
    SyncGet(&scsp->scs);
    scsp->limition=limition;
    scsp->utc=utc;
    scsp->status=EnumValueHelp(Server,Common,Sync,Protection,Status,Active);
    scsp->handler=handler;
    scsp->immediate=immediate;
    scsp->listed=false;
    INIT_LIST_HEAD(&scsp->node);
    INIT_DELAYED_WORK(&scsp->work,Work);

    mutex_lock(&lock);
    list_add_tail(&scsp->node,&nodes);
    scsp->listed=true;
    mutex_unlock(&lock);

    struct timespec64 now;
    ktime_get_real_ts64(&now);
    s64 diff=timespec64_to_jiffies(&utc)-timespec64_to_jiffies(&now);
    if(diff>0)schedule_delayed_work(&scsp->work,diff);
    else if(immediate)schedule_delayed_work(&scsp->work,0);
}

Fn(bool,Server,Common,Sync,Protection,Get)(StructHelp(Server,Common,Sync,Protection)*scsp,struct timespec64 utc)
{
    SyncLock(&scsp->scs)return false;
    if(scsp->status!=EnumValueHelp(Server,Common,Sync,Protection,Status,Active))
    {
        SyncUnlock(&scsp->scs);
        return false;
    }

    if(scsp->utc.tv_sec<utc.tv_sec-5)
    {
        scsp->utc=utc;
        struct timespec64 now;
        ktime_get_real_ts64(&now);
        s64 diff=timespec64_to_jiffies(&utc)-timespec64_to_jiffies(&now);
        if(diff>0)mod_delayed_work(system_wq,&scsp->work,diff);
        else mod_delayed_work(system_wq,&scsp->work,0);
    }

    if(scsp->scs.status==EnumValueHelp(Server,Common,Sync,Status,ProcessDeleting))
        scsp->scs.status=EnumValueHelp(Server,Common,Sync,Status,Active);
    scsp->scs.ref++;

    if(scsp->limition&&!--scsp->limition&&scsp->immediate)
    {
        scsp->immediate=false;
        mod_delayed_work(system_wq,&scsp->work,0);
    }
    SyncUnlock(&scsp->scs);
    return true;
}

Fn(bool,Server,Common,Sync,Protection,Lock)(StructHelp(Server,Common,Sync,Protection)*scsp)
{
    if(!Call(Server,Common,Sync,Lock)(&scsp->scs))
        return false;
    if(scsp->status==EnumValueHelp(Server,Common,Sync,Protection,Status,Active)&&(scsp->limition||scsp->handler))
        return true;
    SyncUnlock(&scsp->scs);
    return false;
}

Fn(void,Server,Common,Sync,Protection,Unlock)(StructHelp(Server,Common,Sync,Protection)*scsp)
{
    SyncUnlock(&scsp->scs);
}

Fn(void,Server,Common,Sync,Protection,Delete)(StructHelp(Server,Common,Sync,Protection)*scsp,struct timespec64 utc)
{
    SyncLock(&scsp->scs)return;
    if(scsp->status==EnumValueHelp(Server,Common,Sync,Protection,Status,Active))
    {
        scsp->utc=utc;
        struct timespec64 now;
        ktime_get_real_ts64(&now);
        s64 diff=timespec64_to_jiffies(&utc)-timespec64_to_jiffies(&now);
        if(scsp->immediate||diff<=0)
        {
            scsp->immediate=false;
            mod_delayed_work(system_wq,&scsp->work,0);
        }
        else mod_delayed_work(system_wq,&scsp->work,diff);
    }
    SyncUnlock(&scsp->scs);
    SyncDelete(&scsp->scs);
}

Fn(void,Server,Common,Sync,Protection,Block)(StructHelp(Server,Common,Sync,Protection)*scsp,struct timespec64 utc)
{
    SyncLock(&scsp->scs)return;
    if(scsp->status==EnumValueHelp(Server,Common,Sync,Protection,Status,Active))
    {
        scsp->limition=0;
        if(scsp->utc.tv_sec<utc.tv_sec-5)scsp->utc=utc;
        struct timespec64 now;
        ktime_get_real_ts64(&now);
        s64 diff=timespec64_to_jiffies(&scsp->utc)-timespec64_to_jiffies(&now);
        if(scsp->immediate||diff<=0)
        {
            scsp->immediate=false;
            mod_delayed_work(system_wq,&scsp->work,0);
        }
        else mod_delayed_work(system_wq,&scsp->work,diff);
    }
    SyncUnlock(&scsp->scs);
}

Fn(void,Server,Common,Sync,Protection,Immediate)(StructHelp(Server,Common,Sync,Protection)*scsp,bool immediate)
{
    SyncLock(&scsp->scs)return;
    if(scsp->status==EnumValueHelp(Server,Common,Sync,Protection,Status,Active))
        scsp->immediate=immediate;
    SyncUnlock(&scsp->scs);
}

Fn(void,Server,Common,Sync,Protection,Increase,Limition)(StructHelp(Server,Common,Sync,Protection)*scsp,u32 limition)
{
    SyncLock(&scsp->scs)return;
    if(scsp->status==EnumValueHelp(Server,Common,Sync,Protection,Status,Active))
        scsp->limition+=limition;
    SyncUnlock(&scsp->scs);
}

Fn(void,Server,Common,Sync,Protection,Decrease,Limition)(StructHelp(Server,Common,Sync,Protection)*scsp,u32 limition)
{
    SyncLock(&scsp->scs)return;
    if(scsp->status==EnumValueHelp(Server,Common,Sync,Protection,Status,Active))
    {
        if(scsp->limition>limition)scsp->limition-=limition;
        else scsp->limition=0;
        if(!scsp->limition&&scsp->immediate)
        {
            scsp->immediate=false;
            mod_delayed_work(system_wq,&scsp->work,0);
        }
    }
    SyncUnlock(&scsp->scs);
}

Fn(u32,Server,Common,Sync,Protection,Get,Limition)(StructHelp(Server,Common,Sync,Protection)*scsp)
{
    u32 limition=0;
    SyncLock(&scsp->scs)return 0;
    limition=scsp->limition;
    SyncUnlock(&scsp->scs);
    return limition;
}

InitLibrary(Server,Common,Sync,Protection)
{
}

QuitLibrary(Server,Common,Sync,Protection)
{
    LIST_HEAD(done);
    StructHelp(Server,Common,Sync,Protection)*scsp,*tmp;
    mutex_lock(&lock);
    list_for_each_entry_safe(scsp,tmp,&nodes,node)
    {
        list_move_tail(&scsp->node,&done);
        scsp->listed=false;
    }
    mutex_unlock(&lock);

    list_for_each_entry_safe(scsp,tmp,&done,node)
    {
        cancel_delayed_work_sync(&scsp->work);
        list_del_init(&scsp->node);
        SyncDelete(&scsp->scs);
    }
}
