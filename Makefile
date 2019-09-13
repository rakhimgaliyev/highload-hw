TARGET = main.out

# XXX: Don't forget backslash at the end of any line except the last one
HDRS = \
	   project/include

SRCS = \
		project/src/main.cpp\
		project/src/config.cpp\
		project/src/handler.cpp

.PHONY: all clean

all: $(SRCS)
	$(CXX) -std=gnu++11 -Wall -Wextra -Werror $(addprefix -I,$(HDRS)) -o $(TARGET) $(CFLAGS) $(SRCS)

clean:
	rm -rf $(TARGET)
