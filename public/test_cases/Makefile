CC = gcc
CXX = g++
AR = ar
LD = ld

CFLAGS = -g -Wall -I./

POBJS = 

TEST_SUITE_MAIN = test_suite_main

INI_SRC = test_ini.c ../ini.c
QUEUE_SRC = test_queue.c
STACK_SRC = test_stack.c
CMM_SRC = test_cmm.c ../cmm/cmm.c 
LOG_SRC = test_log.c ../log.c ../file_if.c ../utils.c
RADIX_SRC = test_radix_tree.c ../radix_tree.c 

TARGET_ALL = $(TEST_SUITE_MAIN)

all: $(TARGET_ALL)

$(TEST_SUITE_MAIN):
	gcc -o $@ $(INI_SRC) $(QUEUE_SRC) $(STACK_SRC) $(CMM_SRC) $(LOG_SRC) $(RADIX_SRC) test_suite_main.c $(CFLAGS) -lpthread -lcunit -static
	
	
clean:
	rm -f *.o ../*.o $(TARGET_ALL)
