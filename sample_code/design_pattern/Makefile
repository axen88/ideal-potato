CC = gcc
CXX = g++
AR = ar
LD = ld

CFLAGS = -g -Wall -I./

POBJS = 

STRTOL_OBJS = strtol.o
SHMMUTEX_OBJS = shmmutex.o
FORK_OBJS = fork.o
PTHREAD_SIGNAL_OBJS = pthread_signal.o
POPEN_READ_OBJS = popen_read.o
POPEN_WRITE_OBJS = popen_write.o
GET_PROCESS_ID_OBJS = get_process_id.o
DIR_SCAN_OBJS = dir_scan.o
SERVICE_DAEMON_OBJS = service_daemon.o
AIO_RW_OBJS = aio_rw.o
AIO_CALLBACK_OBJS = aio_callback.o
FLOCK_OBJS = flock.o

STRTOL = strtol
SHMMUTEX = shmmutex
FORK = fork
PTHREAD_SIGNAL = pthread_signal
POPEN_READ = popen_read
POPEN_WRITE = popen_write
GET_PROCESS_ID = get_process_id
DIR_SCAN = dir_scan
SERVICE_DAEMON = service_daemon
AIO_RW = aio_rw
AIO_CALLBACK = aio_callback
FLOCK = flock


TARGET_ALL = $(STRTOL) $(SHMMUTEX) $(FORK) $(PTHREAD_SIGNAL) \
    $(POPEN_READ) $(POPEN_WRITE) $(GET_PROCESS_ID) $(DIR_SCAN) $(SERVICE_DAEMON) \
    $(AIO_RW) $(AIO_CALLBACK) $(FLOCK)

all: $(TARGET_ALL)

%o:%c
	$(CC) -c $(CFLAGS) $< -o $@

$(STRTOL): $(POBJS) $(STRTOL_OBJS)
	$(CXX) -o $@ $^

$(SHMMUTEX): $(POBJS) $(SHMMUTEX_OBJS)
	$(CXX) -o $@ $^ -lpthread

$(FORK): $(POBJS) $(FORK_OBJS)
	$(CXX) -o $@ $^

$(PTHREAD_SIGNAL): $(POBJS) $(PTHREAD_SIGNAL_OBJS)
	$(CXX) -o $@ $^ -lpthread

$(POPEN_READ): $(POBJS) $(POPEN_READ_OBJS)
	$(CXX) -o $@ $^

$(POPEN_WRITE): $(POBJS) $(POPEN_WRITE_OBJS)
	$(CXX) -o $@ $^

$(GET_PROCESS_ID): $(POBJS) $(GET_PROCESS_ID_OBJS)
	$(CXX) -o $@ $^

$(DIR_SCAN): $(POBJS) $(DIR_SCAN_OBJS)
	$(CXX) -o $@ $^

$(SERVICE_DAEMON): $(POBJS) $(SERVICE_DAEMON_OBJS)
	$(CXX) -o $@ $^

$(AIO_RW): $(POBJS) $(AIO_RW_OBJS)
	$(CXX) -o $@ $^ -lrt

$(AIO_CALLBACK): $(POBJS) $(AIO_CALLBACK_OBJS)
	$(CXX) -o $@ $^ -lrt

$(FLOCK): $(POBJS) $(FLOCK_OBJS)
	$(CXX) -o $@ $^ 

clean:
	rm -f *.o $(TARGET_ALL)
