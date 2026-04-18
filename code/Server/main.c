#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wsequence-point"
#pragma GCC diagnostic ignored "-Wmissing-prototypes"
#pragma GCC diagnostic ignored "-Wunused-function"
#include ".setup"
#include "Common/.setup"

static void Start(void)
{
    LibraryInit(Server,Debug)
    LibraryInit(Server,Common)
    LibraryInit(Server,Hardware)
    LibraryInit(Server,Network)
    LibraryInit(Server,OSI)
}

static void Quit(void)
{
    LibraryQuit(Server,OSI)
    LibraryQuit(Server,Network)
    LibraryQuit(Server,Hardware)
    LibraryQuit(Server,Common)
    LibraryQuit(Server,Debug)
}

static bool _IsRunning=true;
bool IsRunning(void)
{
    return READ_ONCE(_IsRunning);
}

typedef long unsigned int LongUnsignedInt;
static int server_reboot_notify(struct notifier_block*,LongUnsignedInt,void*)
{
    WRITE_ONCE(_IsRunning,false);
    Quit();
    return NOTIFY_OK;
}

static struct notifier_block reboot_nb={
    .notifier_call=server_reboot_notify
};

static int __init server_Init(void)
{
    int ret=register_reboot_notifier(&reboot_nb);
    if(ret)return ret;
    Start();
    return 0;
}

#ifdef MODULE
module_init(server_Init);
static void __exit server_Exit(void)
{
    WRITE_ONCE(_IsRunning,false);
    Quit();
    unregister_reboot_notifier(&reboot_nb);
}
module_exit(server_Exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pirasath Luxchumykanthan");
MODULE_DESCRIPTION("Kernel module for backend and domains: Promo.Claims, Claims.Promo, Regulation.Community.");
#else
late_initcall(server_Init);
#endif
#pragma GCC diagnostic pop
