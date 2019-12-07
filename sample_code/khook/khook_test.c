#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/cpu.h>
#include <linux/module.h>

#include "khook.h"

#include <net/tcp.h>

struct nf_hook_ops;
struct sk_buff;
struct net_device;
struct nf_hook_state;


khook_t g_hook;

// stub函数，其入口指令会被替换，调用此函数相当于调用orig函数
static unsigned int stub_ipv4_conntrack_in(const struct nf_hook_ops *ops, struct sk_buff *skb, const struct net_device *in, const struct net_device *out, const struct nf_hook_state *state)
{
	printk("hook stub conntrack\n");
	return 0;
}

// hook函数，当内核在调用orig函数的时候，将会到达这个函数。
static unsigned int hook_ipv4_conntrack_in(const struct nf_hook_ops *ops, struct sk_buff *skb, const struct net_device *in, const struct net_device *out, const struct nf_hook_state *state)
{
	printk("hook conntrack\n");
    
	// 仅仅打印一行信息后，调用orig函数。
	return ((unsigned int (*)(const struct nf_hook_ops *, struct sk_buff *, const struct net_device *, const struct net_device *, const struct nf_hook_state *))g_hook.stub_func_ptr)(ops, skb, in, out, state);
}


static __init int test_init(void)
{
	s32 ret;

    ret = hook_init();
	if (ret)
    {
		printk("hook init failed(%d)\n", ret);
		return ret;
	}

    ret = hook_create(&g_hook, "ipv4_conntrack_in", (long)hook_ipv4_conntrack_in, (long)stub_ipv4_conntrack_in);
	if (ret)
    {
		printk("hook create failed(%d)\n", ret);
		return ret;
	}

	return 0;
}
module_init(test_init);

static __exit void test_exit(void)
{
	hook_destroy(&g_hook);
    hook_exit();
}
module_exit(test_exit);

MODULE_DESCRIPTION("hook test");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.1");


