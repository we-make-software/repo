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
	{ 0x92997ed8, "_printk" },
	{ 0x98387830, "init_net" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x1935c447, "kmalloc_caches" },
	{ 0x2c6d6d5f, "__kmalloc_cache_noprof" },
	{ 0xba8f4b02, "mutex_init_generic" },
	{ 0x7696f8c7, "__list_add_valid_or_report" },
	{ 0xc7a4fbed, "rtnl_lock" },
	{ 0x548ae822, "netdev_update_features" },
	{ 0xb4210ecf, "dev_add_pack" },
	{ 0x6e720ff2, "rtnl_unlock" },
	{ 0x609f1c7e, "synchronize_net" },
	{ 0x0580f6b6, "dev_remove_pack" },
	{ 0x950eb34e, "__list_del_entry_valid_or_report" },
	{ 0x037a0cba, "kfree" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xe8245ac1, "__pskb_pull_tail" },
	{ 0x04e79dbb, "module_layout" },
};

MODULE_INFO(depends, "");

