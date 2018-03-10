CC = gcc
CXX = g++
AR = ar
LD = ld

CFLAGS = -g -Wall -I./

POBJS = 

TEST_INI = test_ini
TEST_LOG = test_log
TEST_CMM = test_cmm
TEST_QUEUE = test_queue
TEST_STACK = test_stack
TEST_ASYNC_WORK_QUEUE = test_async_work_queue

#TARGET_ALL = $(TEST_INI) $(TEST_LOG) $(TEST_CMM) $(TEST_QUEUE)\
 #    $(TEST_STACK) $(TEST_ASYNC_WORK_QUEUE) 
 
TARGET_ALL = $(TEST_INI) $(TEST_CMM)  $(TEST_QUEUE)\
     $(TEST_STACK)  

all: $(TARGET_ALL)

$(TEST_INI):
	gcc -o $@ test_ini.c ../ini.c $(CFLAGS)
	
$(TEST_LOG):
	gcc -o $@ test_log.c ../log.c $(CFLAGS) -lpthread
	
$(TEST_CMM):
	gcc -o $@ test_cmm.c ../cmm/cmm.c $(CFLAGS) -lpthread
	
$(TEST_QUEUE):
	gcc -o $@ test_queue.c $(CFLAGS) -lpthread
	
$(TEST_STACK):
	gcc -o $@ test_stack.c $(CFLAGS) -lpthread
	
$(TEST_ASYNC_WORK_QUEUE):
	g++ -o $@ test_async_work_queue.cpp $(CFLAGS) -lpthread
	
clean:
	rm -f *.o $(TARGET_ALL)
