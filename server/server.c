#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#include "server.h"
#include "define/.h"
#include "define/.c"
#include "init/.c"
#include "exit/.c"
#ifndef MODULE
    static int reboot_notify(struct notifier_block*,unsigned long,void*){
        WRITE_ONCE(_IsOnline,false);
        Call(exit,execute);
        return NOTIFY_OK;
    }
    static struct notifier_block reboot_nb={.notifier_call=reboot_notify};
#endif
static int __init server_Init(void){
    #ifndef MODULE
        register_reboot_notifier(&reboot_nb);
    #endif
    Call(init,execute);
    return 0;
}
#ifdef MODULE
    module_init(server_Init);
    static void __exit server_Exit(void){
        WRITE_ONCE(_IsOnline,false);
        Call(exit,execute);
    }
    module_exit(server_Exit);
#else
    late_initcall(server_Init);
#endif
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pirasath Luxchumykanthan");
MODULE_DESCRIPTION("Kernel module for backend and domains: Promo.Claims & Claims.Promo & Regulation.Community");
#pragma GCC diagnostic pop