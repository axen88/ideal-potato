CC = gcc
CXX = g++
AR = ar
LD = ld

CFLAGS = -g -Wall -I./

POBJS = a.o b.o

MY_LIB_OBJS = $(POBJS)
MY_TEST_OBJS= test.o

MY_LIB = libmy.a
MY_TEST= test
MY_TEST_LIB= test_lib

TARGET_ALL = $(MY_LIB) $(MY_TEST) $(MY_TEST_LIB)

all: $(TARGET_ALL)

%o:%c
	$(CC) -c $(CFLAGS) $< -o $@

%o:%cpp
	$(CXX) -c $(CFLAGS) $< -o $@

$(MY_LIB): $(MY_LIB_OBJS)
	$(AR) r $@ $^

$(MY_TEST): $(POBJS) $(MY_TEST_OBJS)
	$(CXX) -o $@ $^

$(MY_TEST_LIB): $(MY_TEST_OBJS) $(MY_LIB)
	$(CXX) -o $@ $^

$(MY_TEST_SO1): $(MY_TEST_OBJS) $(MY_SO)
	$(CXX) -o $@ $^

$(MY_TEST_SO2): $(MY_TEST_OBJS)
	$(CXX) -o $@ $^ -L./ -lmy

clean:
	rm -f *.o $(TARGET_ALL)
