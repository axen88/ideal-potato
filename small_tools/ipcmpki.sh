#!/bin/bash

set -x

opt_cpu=0; opt_pid=0; opt_cmd=0; cpu=""; pid=""; cmd=""

usage()
{
	
 	echo "USAGE: ipcmpki {-C CPU | -p PID | -c CMD} [interval [duration]]}"

	exit
}

while getopts C:p:c:h opt
do
	case $opt in
		C) opt_cpu=1; cpu=$OPTARG ;;
		p) opt_pid=1; pid=$OPTARG ;;
		c) opt_cmd=1; cmd=$OPTARG ;;
		h|?)  usage ;;
	esac
done
shift $(( $OPTIND - 1 ))

if [ `expr $opt_cpu + $opt_pid + $opt_cmd` -gt 1 ]; then
	echo >$2 "ERROR: pick one of -C, -p -c"
	usage
fi
secs=${1-1}
duration=${2-999999999}
target=-a
[ $cpu ] && target="-C $cpu "
[ $pid ] && target="-p $pid "
[ $cmd ] && target="$cmd "

if [ $pid ]; then
	if [ ! -d /proc/$pid ]; then
		echo >$2 "ERROR: Can't find PID $pid. Exiting."
		exit
	fi
fi

echo perf stat -e cycles -e cache-misses -e instructions -I $(( secs * 1000 )) $target
perf stat -e cycles -e cache-misses -e instructions -I $(( secs * 1000 )) $target 2>&1 | awk '
	BEGIN {
		htxt = sprintf("%-10s %-10s %-10s %5s %5s",
		"K_CYCLES", "K_INSTR", "K_CHMISS", "IPC", "MPKI");
		print htxt
	}
	/invalid/ { print $0 } 
	{ gsub(/,/, ""); }
	$3 == "cycles" { cycles = $2; }
	$3 == "cache-misses" { cachemiss = $2; }
	$3 == "instructions" {
		ins = $2
		if (cycles == 0) { cycles = 1}
		printf("%-10d %-10d %-10d %5.2f %5.2f\n",
			cycles / 1000, ins / 1000, cachemiss / 1000,
			ins/cycles, 1000 * (cachemiss) / ins)
	}
'







