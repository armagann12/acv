INCLUDE_DIRS = -I/usr/include/opencv4
LIB_DIRS = 
CC = gcc

LIBS= -L/usr/lib -lopencv_core -lopencv_flann -lopencv_video -lrt

CDEFS=
CFLAGS= -O3 $(INCLUDE_DIRS) $(CDEFS)
LIBS=-lpthread

PRODUCT=sharpen_grid sharpen

HFILES= 
CFILES= sharpen_grid.c

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.c=.o}

all:	${PRODUCT}

clean:
	-rm -f *.o *.NEW *~
	-rm -f ${PRODUCT} ${DERIVED} ${GARBAGE}

sharpen: sharpen.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ sharpen.o $(LIBS)

sharpen_grid: sharpen_grid.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ sharpen_grid.o $(LIBS)

opencv_version: opencv_version.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o `pkg-config --libs opencv4` $(LIBS)

depend:

.c.o:
	$(CC) $(CFLAGS) -c $<
