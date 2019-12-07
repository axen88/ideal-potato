#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/cpu.h>
#include <linux/module.h>

#include "khook.h"


static void *(*ptr_poke_smp)(void *addr, const void *opcode, size_t len);


int hook_create(khook_t *h, char *orig_func_name, long hook_func_ptr, long stub_func_ptr)
{
	s32 hook_offset, orig_offset;
    long orig_func_ptr;

	// 找到要hook的函数
	orig_func_ptr = kallsyms_lookup_name(orig_func_name);
	if (!orig_func_ptr) {
		printk("lookup %s failed\n", orig_func_name);
		return -1;
	}

	// orig信息保存
    h->orig_func_ptr = orig_func_ptr;
	// 保存orig函数的入口指令
	h->orig_op[0] = ((char*)orig_func_ptr)[0];
	h->orig_op[1] = ((char*)orig_func_ptr)[1];
	h->orig_op[2] = ((char*)orig_func_ptr)[2];
	h->orig_op[3] = ((char*)orig_func_ptr)[3];
	h->orig_op[4] = ((char*)orig_func_ptr)[4];

	// hook信息初始化
	h->hook_func_ptr = hook_func_ptr;
	// 设置从orig函数跳转到hook函数的指令
	h->hook_op[0] = 0xe9; // 第一个字节当然是jump
	// 计算目标hook函数到orig函数的相对偏移
	hook_offset = (s32)(hook_func_ptr - orig_func_ptr - OPTSIZE);
	// 后面4个字节为一个相对偏移
	(*(s32*)(&h->hook_op[1])) = hook_offset;

	// stub信息初始化
	h->stub_func_ptr = stub_func_ptr;
	// 设置从stub函数跳转到orig函数的指令
	h->stub_op[0] = 0xe9; // 第一个字节当然是jump
	// 计算目标orig函数将要执行的位置到当前位置的偏移
	orig_offset = (s32)(orig_func_ptr + OPTSIZE - (stub_func_ptr + OPTSIZE));
	(*(s32*)(&h->stub_op[1])) = orig_offset;


	// 替换操作！
	get_online_cpus();
	ptr_poke_smp((void *)stub_func_ptr, h->stub_op, OPTSIZE); // stub指令作用在stub函数上，以跳转到orig函数上
	barrier();
	ptr_poke_smp((void *)orig_func_ptr, h->hook_op, OPTSIZE); // hook指令作用在orig函数上，以跳转到hook函数上
	put_online_cpus();

    return 0;
}

void hook_destroy(khook_t *h)
{
    if (h->orig_func_ptr == 0)
    {
        return;
    }
    
	get_online_cpus();
	ptr_poke_smp((void *)h->orig_func_ptr, h->orig_op, OPTSIZE); // 还原orig函数入口指令
	barrier();
	put_online_cpus();
}

int hook_init(void)
{
	// 这个poke函数完成的就是重映射，写text段的事
	ptr_poke_smp = (void *(*)(void *, const void *, size_t))kallsyms_lookup_name("text_poke_smp");
	if (!ptr_poke_smp) {
		printk("lookup %s failed\n", "text_poke_smp");
		return -1;
	}

	return 0;
}

void hook_exit(void)
{
    
}

