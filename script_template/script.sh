#!/bin/bash

# 本脚本生成的日志
LOG_DIR=.
LOG_NAME=$2.log
LOG_FULL_NAME=$LOG_DIR/$LOG_NAME

# 重命名日志文件
rename_log()
{
	timeStamp=`date +"%04Y%02m%02d_%02k%02M%02S"`
	if [ -f $1/$2 ]; then
		#echo mv $1/$2 $1/$timeStamp"_$2"
		mv $1/$2 $1/$timeStamp"_$2"
	fi
}

# 开始脚本
script_start()
{
	while [ 1 ]; do
		echo -n [`date +"%04Y-%02m-%02d %02k:%02M:%02S"`]" "
		echo script is running...
		
		sleep 1
	done
}

# 停止脚本
script_stop()
{
	ps -ef | grep $0 | grep -v "grep\|stop" | awk {'print $2'} | xargs kill -9 >/dev/null 1>/dev/null 2>/dev/null
}

# 查询脚本是否已经在运行
script_status()
{
	ps -ef | grep $0 | grep -v "grep\|status"
}

# 主程序
main()
{
	case "$1" in
		start)
			# 重命名之前的日志文件
			rename_log $LOG_DIR $LOG_NAME
			
			script_start >$LOG_FULL_NAME 1>$LOG_FULL_NAME 2>$LOG_FULL_NAME &
			;;
			
		status)
			script_status
			;;
		
		stop)
			script_stop
			;;
			
		*)
			echo "usage: $0 start|status|stop"
			;;
	esac
}

main $*
