APP=scarter_engine
CC=g++
CFLAGS=-Wall `pkg-config --cflags gtkmm-2.4 glibmm-2.4 gtkglextmm-1.2`
LDFLAGS=`pkg-config --libs gtkmm-2.4 glibmm-2.4 gtkglextmm-1.2`

DEPS= Application.h   \
	  GLConfigUtil.h  \
	  SimpleScene.h

SRCS= Application.cpp \
	  SimpleScene.cpp \
	  main.cpp

OBJS= $(SRCS:.cpp=.o)

all: $(APP)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

# it is important that $(OBJS) stands _before_ $(LDFLAGS)
$(APP):	$(OBJS) $(DEPS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(APP)

clean:
	rm -f *.o  *~ $(APP)
