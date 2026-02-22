#ifndef server_h
    #define server_h
    #include <linux/module.h>
    #include <linux/kernel.h>
    #include <linux/notifier.h>
    #include <linux/reboot.h>
    #include <linux/ethtool.h>
    #include <linux/netdevice.h>
    #include <linux/skbuff.h>
    #include <linux/etherdevice.h>
    #include <linux/if_packet.h>
    #include <linux/ktime.h>
    #include <linux/timekeeping.h>
    #include <linux/slab.h>
    #include <linux/types.h>
    #include <linux/atomic.h>
    #include <linux/blkdev.h>
    #include <linux/mutex.h>
    #include <linux/rtnetlink.h>
    
    #ifdef __INTELLISENSE__
        #define MODULE_LICENSE(x)
        #define MODULE_AUTHOR(x)
        #define MODULE_DESCRIPTION(x)
        typedef struct{s64 counter;}atomic64_t;
        #define READ_ONCE(x)(x)
        #define WRITE_ONCE(x,v)((x) = (v))
    #endif

    #define Inline(type)\
            static inline type __attribute__((always_inline,flatten,__unused__))

    #define Function(type,project,name,...)\
            Inline(type) project##_##name(__VA_ARGS__)

    #define Void(project,name,...)\
            Function(void,project,name,__VA_ARGS__)

    #define DefineFunction(type,project,name,...)\
            Function(type,project,name,__VA_ARGS__)

    #define DefineVoid(project,name,...)\
            Void(project,name,__VA_ARGS__)

    #define Call(project,name,...)\
            project##_##name(__VA_ARGS__)

    #define DefineStruct(name)\
        struct s_##name;\
        typedef struct s_##name name;\
        struct s_##name

    #define DefineNetworkStruct(name)\
        struct __attribute__((packed))ns_##name;\
        typedef struct ns_##name name;\
        struct __attribute__((packed))ns_##name

    #define DefineDiskStruct(name)\
        struct __aligned((4096))ds_##name;\
        typedef struct ds_##name name;

    #define NetworkAddStruct\
        struct __attribute__((packed))

    #define DiskStruct(name)\
        struct __aligned((4096))ds_##name
    
    typedef struct mutex Lock;

    
    #define WaitForLock(lock)\
            mutex_lock(&(lock))

    #define ReleaseLock(lock)\
            mutex_unlock(&(lock))

    typedef struct sk_buff*buffer;
    typedef struct list_head list;

    #define InitList(node)\
        INIT_LIST_HEAD(&(node))

    #define AddList(node,head)\
        list_add(&(node),&(head))

    #define UpdateList(node,head)\
        list_move(&(node),&(head))

    #define Set(obj,field,value)\
        (obj)->field=(value)
    
    #define Clear(obj,field,mask)\
        (obj)->field&=~(mask)

    #define InitLock(lock)\
        mutex_init(&(lock))
    static bool _IsOnline=true;
   
    #define OnlineStatus\
        READ_ONCE(_IsOnline)
    
#define InitGlobalCache(type)\
    static struct kmem_cache*type##_cache;\
    DefineVoid(Cache,InitCache_##type,void){\
        type##_cache=kmem_cache_create(#type,sizeof(type),0,SLAB_HWCACHE_ALIGN,NULL);\
        BUG_ON(!type##_cache);\
    }\
    DefineVoid(Cache,ExitCache_##type,void){\
        kmem_cache_destroy(type##_cache);\
    }\
    DefineFunction(type*,Cache,Alloc_##type,void){\
        return kmem_cache_alloc(type##_cache,GFP_ATOMIC);\
    }\
    DefineVoid(Cache,Free_##type,type*ptr){\
        kmem_cache_free(type##_cache,ptr);\
    }
    
    #define InitCache(type)\
            InitCache_##type()

    #define ExitCache(type)\
            ExitCache_##type()

    #define CacheAlloc(type)\
            Alloc_##type()

    #define CacheFree(type,ptr)\
            Free_##type(ptr)
#endif