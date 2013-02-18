APP=scarter_engine
CC=g++

LIBS= gtkmm-2.4       		\
	  glibmm-2.4      		\
	  gtkglextmm-1.2  		\
	  glew

CFLAGS=-g -Wall -std=c++11 `pkg-config --cflags $(LIBS)`
LDFLAGS=`pkg-config --libs $(LIBS)`

DEPS= GLConfigUtil.h		\
	  math3d.h        		\
	  Gui.h           		\
	  Scenes/SceneBase.hpp	\
	  Scenes/SceneMorph.hpp

SRCS= main.cpp				\
	  Gui.cpp        		\
	  Scenes/SceneBase.cpp	\
	  Scenes/SceneMorph.cpp


OBJS= $(SRCS:.cpp=.o)

all: $(APP)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

# it is important that $(OBJS) stands _before_ $(LDFLAGS)
$(APP):	$(OBJS) $(DEPS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(APP)

clean:
	rm -f *.o  *~ $(APP)
