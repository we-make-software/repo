#include ".setup"

static LIST_HEAD(nodes);
static DEFINE_MUTEX(lock);

static void Work(struct work_struct*thread)
{
    StructHelp(Server,Common,Sync,Protection)*scsp=container_of(to_delayed_work(thread),StructHelp(Server,Common,Sync,Protection),work);
    SyncLock(&scsp->scs)
    {
        SyncDelete(&scsp->scs);
        return;
    }
    scsp->limition=0;

    mutex_lock(&lock);
    list_del(&scsp->node);
    mutex_unlock(&lock);

    SyncUnlock(&scsp->scs);
    SyncDelete(&scsp->scs);
}

Fn(void,Server,Common,Sync,Protection,Setup)(StructHelp(Server,Common,Sync,Protection)*scsp,u32 limition,struct timespec64 utc,void(*before)(StructHelp(Server,Common,Sync)*),void(*after)(StructHelp(Server,Common,Sync)*))
{
    SyncSetup(&scsp->scs,before,after);
    SyncLock(&scsp->scs)return;

    mutex_lock(&lock);
    list_add_tail(&scsp->node,&nodes);
    mutex_unlock(&lock);

    SyncGet(&scsp->scs);
    scsp->limition=limition;
    scsp->utc=utc;
    INIT_DELAYED_WORK(&scsp->work,Work);

    struct timespec64 now;
    ktime_get_real_ts64(&now);
    s64 diff=timespec64_to_jiffies(&utc)-timespec64_to_jiffies(&now);
    schedule_delayed_work(&scsp->work,diff);
    SyncUnlock(&scsp->scs);
}

Fn(bool,Server,Common,Sync,Protection,Get)(StructHelp(Server,Common,Sync,Protection)*scsp,struct timespec64 utc)
{
    SyncLock(&scsp->scs)return false;
    if(delayed_work_pending(&scsp->work)&&(s64)(scsp->work.timer.expires-jiffies)<msecs_to_jiffies(50))
    {
        SyncUnlock(&scsp->scs);
        return false;
    }
    if(!scsp->limition)
    {
        SyncUnlock(&scsp->scs);
        return false;
    }
    s64 diff=utc.tv_sec-scsp->utc.tv_sec;
    if(diff>5)
    {
        scsp->utc=utc;
        struct timespec64 now;
        ktime_get_real_ts64(&now);
        diff=timespec64_to_jiffies(&utc)-timespec64_to_jiffies(&now);
        mod_delayed_work(system_wq,&scsp->work,diff);
    }
    if(SyncGet(&scsp->scs))
    {
        scsp->limition--;
        SyncUnlock(&scsp->scs);
        return true;
    }
    SyncUnlock(&scsp->scs);
    return false;
}

Fn(bool,Server,Common,Sync,Protection,Lock)(StructHelp(Server,Common,Sync,Protection)*scsp)
{
    if(!Call(Server,Common,Sync,Lock)(&scsp->scs))
        return false;
    if(scsp->limition)
        return true;
    SyncUnlock(&scsp->scs);
    SyncDelete(&scsp->scs);
    return false;
}

Fn(void,Server,Common,Sync,Protection,Unlock)(StructHelp(Server,Common,Sync,Protection)*scsp)
{
    SyncUnlock(&scsp->scs);
}

Fn(void,Server,Common,Sync,Protection,Delete)(StructHelp(Server,Common,Sync,Protection)*scsp,u32 limition,struct timespec64 utc)
{
    SyncLock(&scsp->scs)return;
    scsp->limition+=limition;
    s64 diff=utc.tv_sec-scsp->utc.tv_sec;
    if(diff>5)
    {
        scsp->utc=utc;
        struct timespec64 now;
        ktime_get_real_ts64(&now);
        diff=timespec64_to_jiffies(&utc)-timespec64_to_jiffies(&now);
        mod_delayed_work(system_wq,&scsp->work,diff);
    }
    SyncUnlock(&scsp->scs);
    SyncDelete(&scsp->scs);
}

Fn(void,Server,Common,Sync,Protection,Block)(StructHelp(Server,Common,Sync,Protection)*scsp,struct timespec64 utc)
{
    SyncLock(&scsp->scs)return;
    scsp->limition=0;
    s64 diff=utc.tv_sec-scsp->utc.tv_sec;
    if(diff>5)
    {
        scsp->utc=utc;
        struct timespec64 now;
        ktime_get_real_ts64(&now);
        diff=timespec64_to_jiffies(&utc)-timespec64_to_jiffies(&now);
        mod_delayed_work(system_wq,&scsp->work,diff);
    }
    SyncUnlock(&scsp->scs);
}

InitLibrary(Server,Common,Sync,Protection)
{
}

QuitLibrary(Server,Common,Sync,Protection)
{
    StructHelp(Server,Common,Sync,Protection)*scsp,*tmp;
    mutex_lock(&lock);
    list_for_each_entry_safe(scsp,tmp,&nodes,node)
    {
        cancel_delayed_work_sync(&scsp->work);
        list_del(&scsp->node);
        SyncDelete(&scsp->scs);
    }
    mutex_unlock(&lock);
}
