INCLUDE_DIRS = -I/usr/include/opencv4
LIB_DIRS = 
CC=g++

CDEFS=
CFLAGS= -O0 -g $(INCLUDE_DIRS) $(CDEFS)
LIBS= -L/usr/lib -lopencv_core -lopencv_flann -lopencv_video -lrt

HFILES= 
CFILES= simpler_capture2.cpp

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.cpp=.o}

all:	simpler_capture2

clean:
	-rm -f *.o *.d
	-rm -f ex_00

simpler_capture2: simpler_capture2.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o `pkg-config --libs opencv4` $(LIBS)

depend:

.cpp.o: $(SRCS)
	$(CC) $(CFLAGS) -c $<
