#include <linux/init.h>
#include <linux/module.h>

static int hello_init(void)
{
	printk(KERN_ALERT "Hello to kernel world.\n");
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "Good bye, kernel world.\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("axen");

module_init(hello_init);
module_exit(hello_exit);

