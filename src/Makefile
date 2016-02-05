CC = gcc
CXX = g++
AR = ar
LD = ld

CFLAGS = -g -Wall -I./

POBJS = 

AVP_LIST_DOUBLE = test_list_double
AVP_LIST_SINGLE = test_list_single
AVP_INI = test_ini
AVP_LOG = test_log
AVP_MEM = test_mem
AVP_QUEUE_C = test_queue_c
AVP_QUEUE_CPP = test_queue_cpp
AVP_STACK_C = test_stack_c
AVP_STACK_CPP = test_stack_cpp
AVP_THREADS_GROUP = test_threads_group
AVP_ASYNC_WORK_QUEUE = test_async_work_queue

TARGET_ALL = $(AVP_LIST_DOUBLE) $(AVP_LIST_SINGLE) $(AVP_INI) $(AVP_LOG) $(AVP_MEM) $(AVP_QUEUE_C) $(AVP_QUEUE_CPP) \
     $(AVP_STACK_C) $(AVP_STACK_CPP) $(AVP_THREADS_GROUP) \
    $(AVP_ASYNC_WORK_QUEUE) 

all: $(TARGET_ALL)

$(AVP_LIST_DOUBLE):
	gcc -o $@ avp_list_double.c $(CFLAGS) -D_EN_DEBUG_

$(AVP_LIST_SINGLE):
	gcc -o $@ avp_list_single.c $(CFLAGS) -D_EN_DEBUG_
	
$(AVP_INI):
	gcc -o $@ avp_ini.c $(CFLAGS) -D_EN_DEBUG_
	
$(AVP_LOG):
	gcc -o $@ avp_log.c avp_mutex.c $(CFLAGS) -D_EN_DEBUG_ -lpthread
	
$(AVP_MEM):
	gcc -o $@ avp_mem.c avp_mutex.c $(CFLAGS) -D_EN_DEBUG_ -lpthread
	
$(AVP_QUEUE_C):
	gcc -o $@ avp_queue.c avp_mutex.c $(CFLAGS) -D_EN_DEBUG_ -lpthread
	
$(AVP_QUEUE_CPP):
	g++ -o $@ avp_queue.cpp avp_mutex.c $(CFLAGS) -D_EN_DEBUG_ -lpthread
	
$(AVP_STACK_C):
	gcc -o $@ avp_stack.c avp_mutex.c $(CFLAGS) -D_EN_DEBUG_ -lpthread
	
$(AVP_STACK_CPP):
	g++ -o $@ avp_stack.cpp avp_mutex.c $(CFLAGS) -D_EN_DEBUG_ -lpthread
	
$(AVP_THREADS_GROUP):
	gcc -o $@ avp_threads_group.c $(CFLAGS) -D_EN_DEBUG_ -lpthread
	
$(AVP_ASYNC_WORK_QUEUE):
	g++ -o $@ avp_async_work_queue.cpp avp_mutex.c avp_queue.c avp_threads_group.c $(CFLAGS) -D_EN_ASYNC_ -lpthread
	
clean:
	rm -f *.o $(TARGET_ALL)
