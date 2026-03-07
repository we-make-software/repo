#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#include "core/.setup"
#include "core/.init"
static bool _IsOnline=true;
bool IsOnline(void)
{
    return READ_ONCE(_IsOnline);
}
static int server_reboot_notify(struct notifier_block*nb, unsigned long action, void*data)
{
    WRITE_ONCE(_IsOnline, false);
    Exit(core);
    return NOTIFY_OK;
}

static struct notifier_block reboot_nb = {
    .notifier_call = server_reboot_notify
};

static int __init server_Init(void)
{
    int ret=register_reboot_notifier(&reboot_nb);
    if(ret)
        return ret;
    Init(core);
    return 0;
}

#ifdef MODULE
module_init(server_Init);

static void __exit server_Exit(void)
{
    WRITE_ONCE(_IsOnline, false);
    Exit(core);
    unregister_reboot_notifier(&reboot_nb);
}
module_exit(server_Exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pirasath Luxchumykanthan");
MODULE_DESCRIPTION(
    "Kernel module for backend and domains: Promo.Claims, Claims.Promo, Regulation.Community. "
    "Administrator: C and C++ Projects and tasks on Facebook: https://www.facebook.com/c89c99. "
    "Focus: low level programming and reverse engineering for the Linux kernel, C89 and C99. "
    "Supported by Linux kernel 5.15 and above. "
    "I use Linux 6.19.0+. "
    "This is my own Linux build, not Kali, Debian, or anything else."
);
#else
late_initcall(server_Init);
#endif

#pragma GCC diagnostic pop