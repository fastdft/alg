ifeq ($(strip $(BUILD_TYPE)),)
BUILD_TYPE=debug
endif

AR=ar
CC=gcc
CXX=g++

CFLAGS = -g

LDFLAGS = -lm -lstdc++

LIB_OBJS = \
	calc.o \
	dp.o \
	palindrome.o \
	ReadData.o \
	sort.o \
	tree.o \
	main.o \
	SbTree.o \
	state_machine.o \
	str_data.o \
	wbt.o \
	interval_tree.o \
	llrb.o \
	package.o \
	recursive.o \
	stack.o \
	str_alg.o \
	utils.o \
	time_statistics.o \


CXXFLAGS=$(CFLAGS)

%.o : %.c
	$(CC) -c $(CFLAGS) -o $@ $<

%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

TARGET=alg

.PHONY:all
all : $(TARGET)
$(TARGET):$(TEST_OBJS) $(LIB_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY:clean
clean:
	rm -rf $(TARGET) $(TEST_OBJS)
	rm -rf *.o	
