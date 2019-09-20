TARGET = main.out

# XXX: Don't forget backslash at the end of any line except the last one
HDRS = \
	   project/include

SRCS = \
		project/src/main.cpp\
		project/src/config.cpp\
		project/src/handler.cpp\
		project/src/session.cpp\
		project/src/epoll.cpp\
		project/src/threadpool.cpp\
		project/src/server.cpp

.PHONY: all clean

all: $(SRCS)
	$(CXX) -std=gnu++11 -pthread -Wall -Wextra -Wno-catch-value -Wno-unused-parameter -Werror $(addprefix -I,$(HDRS)) -o $(TARGET) $(CFLAGS) $(SRCS)

clean:
	rm -rf $(TARGET)
