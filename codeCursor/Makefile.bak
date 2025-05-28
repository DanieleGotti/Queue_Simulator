CC	      = g++

#CFLAGS	      = -O4 -DNDEBUG -funroll-loops
CFLAGS	      = -g	

LIBFLAG       = -lm

EXTHDRS	      = /usr/include/assert.h \
		/usr/include/floatingpoint.h \
		/usr/include/math.h \
		/usr/include/stdio.h \
		/usr/include/sys/ieeefp.h

HDRS	      = global.h \
		queue.h \
		rand.h \
		simulator.h \
		calendar.h \
		packet.h \
		event.h \
		easyio.h \
		stat.h \
		buffer.h 
 
MAKEFILE      = Makefile

OBJS	      = main.o \
		queue.o \
		rand.o \
		simulator.o \
		calendar.o \
		packet.o \
		event.o \
		easyio.o \
		stat.o \
		buffer.o 

PROGRAM	      = queue 


SRCS	      = main.cpp \
		queue.cpp \
		rand.cpp \
		simulator.cpp \
		calendar.cpp \
		packet.cpp \
		event.cpp \
		easyio.cpp \
		stat.cpp \
		buffer.cpp

all:		$(PROGRAM)

$(PROGRAM):     $(OBJS) $(LIBS)
		@echo -n "Loading $(PROGRAM) ... "
		@$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(PROGRAM) $(LIBFLAG)
		@echo "done"

clean:;		@rm -f $(OBJS)

depend:;	@mkmf -f $(MAKEFILE) PROGRAM=$(PROGRAM)

index:;		@ctags -wx $(HDRS) $(SRCS)

tags:           $(HDRS) $(SRCS); @ctags $(HDRS) $(SRCS)

###
main.o: global.h simulator.h queue.h
queue.o: global.h simulator.h queue.h event.h
rand.o: global.h rand.h
simulator.o: global.h 
calendar.o: calendar.h global.h
packet.o: packet.h global.h
buffer.o: buffer.h global.h
event.o: global.h event.h
# DO NOT DELETE
