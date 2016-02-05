CC = gcc
CXX = g++
AR = ar
LD = ld

CFLAGS = -g -Wall -I./

POBJS = 

READ_STR_OBJS = read_str.o
WRITE_STR_OBJS = write_str.o
READ_HEX_OBJS = read_hex.o
FTRUNCATE_OBJS = ftruncate.o
IOCTL_OBJS = ioctl.o

READ_STR = read_str
WRITE_STR = write_str
READ_HEX = read_hex
FTRUNCATE = ftruncate
IOCTL = ioctl

TARGET_ALL = $(READ_STR) $(WRITE_STR) $(READ_HEX) $(FTRUNCATE) $(IOCTL)

all: $(TARGET_ALL)

%o:%c
	$(CC) -c $(CFLAGS) $< -o $@

$(READ_STR): $(POBJS) $(READ_STR_OBJS)
	$(CXX) -o $@ $^

$(READ_HEX): $(POBJS) $(READ_HEX_OBJS)
	$(CXX) -o $@ $^

$(WRITE_STR): $(POBJS) $(WRITE_STR_OBJS)
	$(CXX) -o $@ $^

$(FTRUNCATE): $(POBJS) $(FTRUNCATE_OBJS)
	$(CXX) -o $@ $^

$(IOCTL): $(POBJS) $(IOCTL_OBJS)
	$(CXX) -o $@ $^

clean:
	rm -f *.o $(TARGET_ALL)
