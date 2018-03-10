# ideal-potato
my source code library


所属模块	文件名	功能说明

基本头文件

	osp_types.h	基本类型定义

	osp_error.h	错误定义

	osp_debug.h	调试信息定义

	osp_macro.h	常用宏操作定义

基本模块

基本操作	osp_time	时间类函数

	osp_mutex	锁操作

	osp_list	list数据结构

	osp_map	map数据结构

	osp_stack	stack数据结构

	osp_queue	queue数据结构

文件操作	osp_file	文件操作

	osp_dir	目录操作

上层通用模块

	osp_print	屏幕打印相关操作

	osp_log	日志模块

	osp_event	事件告警模块

	osp_ini	Ini文件读写模块

	osp_cache	简单的文件cache机制

	osp_bitmem	内存中的bit操作

	osp_bitfile	文件中的bit操作

	osp_threadspool	线程池

		

常用模块函数或程序

	osp_count	计数示例程序

	osp_thread	多线程示例程序

	osp_aio	异步读写示例程序

	osp_test	测试程序模板

	Makefile	Makefile模板

	daemon_script	守护进程(脚本)模板

	osp_daemon	守护进程(程序)模板





日志级别

内核的日志级别

宏名称	调试级别	说明

KERN_EMERG	0	紧急消息（导致系统错误）

KERN_ALERT	1	必须立即处理的错误

KERN_CRIT	2	严重错误（硬件或软件）

KERN_ERR	3	错误状况（一般出现在驱动程序上）

KEER_WARNING	4	警告级别（可能导致错误）

KERN_NOTICE	5	不是错误，但是一个重要状况

KERN_INFO	6	报告消息

KERN_DEBUG	7	仅用于调试的信息

