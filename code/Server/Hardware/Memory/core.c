#include ".setup"
struct Block{
    u16 ID;
    struct kmem_cache*cache;
    struct list_head node;
};
static LIST_HEAD(nodes);
static DEFINE_MUTEX(lock);
Fn(void,Server,Hardware,Memory,Prepare)(u16 ID){
    char name[32];
    struct Block*block;
    mutex_lock(&lock);
    list_for_each_entry(block,&nodes,node){
        if(block->ID==ID){
            mutex_unlock(&lock);
            return;
        }
    }
    snprintf(name,sizeof(name),"Block_%u",ID);
    block=kmalloc(sizeof(*block),GFP_KERNEL);
    if(!block){
        mutex_unlock(&lock);
        return;
    }
    block->ID=ID;
    block->cache=kmem_cache_create(name,ID,0,SLAB_HWCACHE_ALIGN,NULL);
    if(!block->cache){
        kfree(block);
        mutex_unlock(&lock);
        return;
    }
    list_add(&block->node,&nodes);
    mutex_unlock(&lock);
}

Fn(void*,Server,Hardware,Memory,Get)(u16 ID){
    struct Block*block;
    list_for_each_entry(block,&nodes,node)
        if(block->ID==ID)
            return kmem_cache_alloc(block->cache,GFP_KERNEL);
    return NULL;
}

Fn(void,Server,Hardware,Memory,Delete)(void*shm,u16 ID){
    struct Block*block;
    list_for_each_entry(block,&nodes,node)
        if(block->ID==ID){
            kmem_cache_free(block->cache,shm);
            return;
        }
}

InitLibrary(Server,Hardware,Memory){
}

QuitLibrary(Server,Hardware,Memory){
    struct Block*block,*tmp;
    mutex_lock(&lock);
    list_for_each_entry_safe(block,tmp,&nodes,node){
        list_del(&block->node);
        kmem_cache_destroy(block->cache);
        kfree(block);
    }
    mutex_unlock(&lock);
}
