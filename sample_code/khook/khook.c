#include <linux/kernel.h>
#include <linux/kallsyms.h>
#include <linux/cpu.h>
#include <linux/module.h>

#include <net/tcp.h>

#define OPTSIZE	5

// orig_op保orig函数的入口指令
char orig_op[OPTSIZE] = {0};
// hook_op保存跳转到hook函数的指令
char hook_op[OPTSIZE] = {0};
// stub_op保存跳转orig函数的指令
char stub_op[OPTSIZE] = {0};

// stub函数入口指针，调用stub函数相当于调用orig函数
static unsigned int (*stub_func_ptr)(const struct nf_hook_ops *, struct sk_buff *, const struct net_device *, const struct net_device *, const struct nf_hook_state *);
// orig函数入口指针，其入口指令会被替换掉，jmp到hook函数
static unsigned int (*orig_func_ptr)(const struct nf_hook_ops *, struct sk_buff *, const struct net_device *, const struct net_device *, const struct nf_hook_state *);
// hook函数入口指针，实现用户功能，并在函数中调用stub函数（相当于orig函数）
static unsigned int (*hook_func_ptr)(const struct nf_hook_ops *, struct sk_buff *, const struct net_device *, const struct net_device *, const struct nf_hook_state *);

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
	return stub_func_ptr(ops, skb, in, out, state);
}

static void *(*ptr_poke_smp)(void *addr, const void *opcode, size_t len);

static __init int hook_init(void)
{
	s32 hook_offset, orig_offset;

	// 这个poke函数完成的就是重映射，写text段的事
	ptr_poke_smp = kallsyms_lookup_name("text_poke_smp");
	if (!ptr_poke_smp) {
		printk("err");
		return -1;
	}

	// 找到要hook的函数
	orig_func_ptr = kallsyms_lookup_name("ipv4_conntrack_in");
	if (!orig_func_ptr) {
		printk("err");
		return -1;
	}

	// 保存orig函数的入口指令
	orig_op[0] = ((char*)orig_func_ptr)[0];
	orig_op[1] = ((char*)orig_func_ptr)[1];
	orig_op[2] = ((char*)orig_func_ptr)[2];
	orig_op[3] = ((char*)orig_func_ptr)[3];
	orig_op[4] = ((char*)orig_func_ptr)[4];

	// hook信息初始化
	hook_func_ptr = hook_ipv4_conntrack_in;
	// 设置从orig函数跳转到hook函数的指令
	// 第一个字节当然是jump
	hook_op[0] = 0xe9;
	// 计算目标hook函数到orig函数的相对偏移
	hook_offset = (s32)((long)hook_func_ptr - (long)orig_func_ptr - OPTSIZE);
	// 后面4个字节为一个相对偏移
	(*(s32*)(&hook_op[1])) = hook_offset;

	// stub信息初始化
	stub_func_ptr = stub_ipv4_conntrack_in;
	// 设置从stub函数跳转到orig函数的指令
	stub_op[0] = 0xe9;
	// 计算目标orig函数将要执行的位置到当前位置的偏移
	orig_offset = (s32)((long)orig_func_ptr + OPTSIZE - ((long)stub_func_ptr + OPTSIZE));
	(*(s32*)(&stub_op[1])) = orig_offset;


	// 替换操作！
	get_online_cpus();
	ptr_poke_smp(stub_func_ptr, stub_op, OPTSIZE); // stub指令作用在stub函数上，以跳转到orig函数上
	barrier();
	ptr_poke_smp(orig_func_ptr, hook_op, OPTSIZE); // hook指令作用在orig函数上，以跳转到hook函数上
	put_online_cpus();

	return 0;
}
module_init(hook_init);

static __exit void hook_exit(void)
{
	get_online_cpus();
	ptr_poke_smp(orig_func_ptr, orig_op, OPTSIZE); // 还原orig函数入口指令
	barrier();
	put_online_cpus();
}
module_exit(hook_exit);

MODULE_DESCRIPTION("hook test");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.1");
