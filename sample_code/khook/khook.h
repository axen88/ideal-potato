#ifndef __KHOOK_H__
#define __KHOOK_H__

#define OPTSIZE	5

// hook结构体
typedef struct 
{

	// orig_op保orig函数的入口指令
	char orig_op[OPTSIZE];
	// hook_op保存跳转到hook函数的指令
	char hook_op[OPTSIZE];
	// stub_op保存跳转orig函数的指令
	char stub_op[OPTSIZE];

	// orig函数入口指针，其入口指令会被替换掉，jmp到hook函数
	long orig_func_ptr;
	// hook函数入口指针，实现用户功能，并在函数中调用stub函数（相当于orig函数）
	long hook_func_ptr;
	// stub函数入口指针，调用stub函数相当于调用orig函数
	long stub_func_ptr;

} khook_t;


int hook_init(void);
void hook_exit(void);
int hook_create(khook_t *h, char *orig_func_name, long hook_func_ptr, long stub_func_ptr);
void hook_destroy(khook_t *h);

#endif
