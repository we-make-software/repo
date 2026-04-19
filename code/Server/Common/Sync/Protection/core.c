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

    if(!scsp->limition&&scsp->event)
    {
        scsp->event(scsp);
        scsp->event=NULL;
    }
    scsp->limition=0;

    mutex_lock(&lock);
    list_del(&scsp->node);
    mutex_unlock(&lock);

    SyncUnlock(&scsp->scs);
    SyncDelete(&scsp->scs);
}

Fn(void,Server,Common,Sync,Protection,Setup)(StructHelp(Server,Common,Sync,Protection)*scsp,u32 limition,struct timespec64 utc,void(*before)(StructHelp(Server,Common,Sync)*),void(*after)(StructHelp(Server,Common,Sync)*),void(*event)(StructHelp(Server,Common,Sync,Protection)*),bool immediate)
{
    SyncSetup(&scsp->scs,before,after);
    SyncLock(&scsp->scs)return;

    mutex_lock(&lock);
    list_add_tail(&scsp->node,&nodes);
    mutex_unlock(&lock);

    SyncGet(&scsp->scs);
    scsp->limition=limition+1;
    scsp->utc=utc;
    scsp->event=event;
    scsp->immediate=immediate;
    INIT_DELAYED_WORK(&scsp->work,Work);

    struct timespec64 now;
    ktime_get_real_ts64(&now);
    s64 diff=timespec64_to_jiffies(&utc)-timespec64_to_jiffies(&now);
    if(diff<0)diff=0;
    schedule_delayed_work(&scsp->work,diff);
    SyncUnlock(&scsp->scs);
}

Fn(bool,Server,Common,Sync,Protection,Get)(StructHelp(Server,Common,Sync,Protection)*scsp,struct timespec64 utc)
{
    SyncLock(&scsp->scs)return false;
    struct timespec64 now;
    ktime_get_real_ts64(&now);
    s64 diff=utc.tv_sec-scsp->utc.tv_sec;
    if(diff>5)
    {
        scsp->utc=utc;
        diff=timespec64_to_jiffies(&utc)-timespec64_to_jiffies(&now);
        if(diff>0)mod_delayed_work(system_wq,&scsp->work,diff);
    }
    if(scsp->limition&&SyncGet(&scsp->scs))
    {
        if(!--scsp->limition&&scsp->immediate)
        {
            scsp->immediate=false;
            mod_delayed_work(system_wq,&scsp->work,0);
            SyncUnlock(&scsp->scs);
            SyncDelete(&scsp->scs);
            return false;
        }
        SyncUnlock(&scsp->scs);
        return true;
    }
    if(scsp->immediate)
    {
        scsp->immediate=false;
        mod_delayed_work(system_wq,&scsp->work,0);
        SyncUnlock(&scsp->scs);
        SyncDelete(&scsp->scs);
        return false;
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
    return false;
}

Fn(void,Server,Common,Sync,Protection,Unlock)(StructHelp(Server,Common,Sync,Protection)*scsp)
{
    SyncUnlock(&scsp->scs);
}

Fn(void,Server,Common,Sync,Protection,Delete)(StructHelp(Server,Common,Sync,Protection)*scsp,struct timespec64 utc)
{
    SyncLock(&scsp->scs)return;

    if(!scsp->limition&&scsp->event)
    {
        scsp->event(scsp);
        scsp->event=NULL;
    }

    if(!scsp->limition&&scsp->immediate)
    {
        scsp->immediate=false;
        mod_delayed_work(system_wq,&scsp->work,0);
    }
    else
    {
        s64 diff=utc.tv_sec-scsp->utc.tv_sec;
        if(diff>5)
        {
            scsp->utc=utc;
            struct timespec64 now;
            ktime_get_real_ts64(&now);
            diff=timespec64_to_jiffies(&utc)-timespec64_to_jiffies(&now);
            if(diff>0)mod_delayed_work(system_wq,&scsp->work,diff);
        }
    }

    SyncUnlock(&scsp->scs);
    SyncDelete(&scsp->scs);
}

Fn(void,Server,Common,Sync,Protection,Block)(StructHelp(Server,Common,Sync,Protection)*scsp,struct timespec64 utc)
{
    SyncLock(&scsp->scs)return;
    scsp->limition=0;
    scsp->utc=utc;
    if(scsp->event)
    {
        scsp->event(scsp);
        scsp->event=NULL;
    }
    if(scsp->immediate)
    {
        scsp->immediate=false;
        mod_delayed_work(system_wq,&scsp->work,0);
    }
    else
    {
        s64 diff=utc.tv_sec-scsp->utc.tv_sec;
        if(diff>5)
        {
            scsp->utc=utc;
            struct timespec64 now;
            ktime_get_real_ts64(&now);
            diff=timespec64_to_jiffies(&utc)-timespec64_to_jiffies(&now);
            if(diff>0)mod_delayed_work(system_wq,&scsp->work,diff);
        }
    }

    SyncUnlock(&scsp->scs);
}

Fn(void,Server,Common,Sync,Protection,Event)(StructHelp(Server,Common,Sync,Protection)*scsp,void(*event)(StructHelp(Server,Common,Sync,Protection)*))
{
    scsp->event=event;
}

Fn(void,Server,Common,Sync,Protection,Immediate)(StructHelp(Server,Common,Sync,Protection)*scsp,bool immediate)
{
    scsp->immediate=immediate;
}

Fn(void,Server,Common,Sync,Protection,Increase,Limition)(StructHelp(Server,Common,Sync,Protection)*scsp,u32 limition)
{
    scsp->limition+=limition;
}

Fn(void,Server,Common,Sync,Protection,Decrease,Limition)(StructHelp(Server,Common,Sync,Protection)*scsp,u32 limition)
{
    if(scsp->limition>limition)scsp->limition-=limition;
    else scsp->limition=0;
}

Fn(u32,Server,Common,Sync,Protection,Get,Limition)(StructHelp(Server,Common,Sync,Protection)*scsp)
{
    return scsp->limition?scsp->limition-1:0;
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
