#include ".setup"
#include "../Memory/.setup"
#include "Config/.setup"
#include "Delete/.setup"
#include "Block/.setup"

static LIST_HEAD(nodes);

Fn(void,Server,Hardware,Storage,Release)(StructMemory(Server,Hardware,Storage)*shs)
{
    SyncDelete(&shs->scs);
}

Fn(StructMemory(Server,Hardware,Storage)*,Server,Hardware,Storage,Get,Or,Null)(u16 storageid)
{
    StructMemory(Server,Hardware,Storage)*shs;
    list_for_each_entry(shs,&nodes,node)
        if(shs->id==storageid)
            if(SyncGet(&shs->scs))
            {
                if(nodes.next!=&shs->node)list_move(&shs->node,&nodes);
                return shs;
            }
    return NULL;
}

Fn(bool,Server,Hardware,Storage,Use)(StructMemory(Server,Hardware,Storage)*shs)
{
    if(!shs)return false;
    return SyncGet(&shs->scs);
}

InitLibrary(Server,Hardware,Storage)
{
    char path[64];
    LibraryInit(Server,Hardware,Storage,Block);
    MemoryPrepare(Server,Hardware,Storage);
    for(u16 i=0;i<256;i++)
    {
        snprintf(path,sizeof(path),"/root/storage/%u.pccprc",i);
        struct file*file=filp_open(path,O_RDWR,0);
        if(!IS_ERR(file))
        {
            MemoryGet(shs,Server,Hardware,Storage)
            {
                filp_close(file,NULL);
                continue;
            }
            shs->id=i;
            shs->file=file;
            shs->size=i_size_read(file_inode(file))>>10;
            SyncSetup(&shs->scs,NULL,NULL);
            INIT_LIST_HEAD(&shs->block_node);
            list_add(&shs->node,&nodes);
        }
    }
    StructMemory(Server,Hardware,Storage)*shs;
    list_for_each_entry(shs,&nodes,node)
    {
        Call(Server,Hardware,Storage,Config,Setup)(shs);
        Call(Server,Hardware,Storage,Delete,Setup)(shs);
    }
}

QuitLibrary(Server,Hardware,Storage)
{
    StructMemory(Server,Hardware,Storage)*shs,*tmp;
    list_for_each_entry_safe(shs,tmp,&nodes,node)
    {
        Call(Server,Hardware,Storage,Delete,Exit)(shs);
        Call(Server,Hardware,Storage,Config,Exit)(shs);
        filp_close(shs->file,NULL);
        list_del(&shs->node);
        MemoryDelete(shs);
    }
}
