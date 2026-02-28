#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0xdc50aae2, "__ref_stack_chk_guard" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x3517383e, "register_reboot_notifier" },
	{ 0xac1a55be, "unregister_reboot_notifier" },
	{ 0x98387830, "init_net" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x012c9b31, "kmem_cache_alloc_noprof" },
	{ 0x48890672, "__kmem_cache_create_args" },
	{ 0xc7a4fbed, "rtnl_lock" },
	{ 0x8008d950, "kmem_cache_destroy" },
	{ 0xba8f4b02, "mutex_init_generic" },
	{ 0x7696f8c7, "__list_add_valid_or_report" },
	{ 0x04e79dbb, "module_layout" },
};

MODULE_INFO(depends, "");

