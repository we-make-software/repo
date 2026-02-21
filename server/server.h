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
        
#endif