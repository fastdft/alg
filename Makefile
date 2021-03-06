ifeq ($(strip $(BUILD_TYPE)),)
BUILD_TYPE=debug
endif

AR=ar
CC=gcc
CXX=g++

CFLAGS = -g -O2

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
	bwt.o \
	interval_tree.o \
	llrb.o \
	package.o \
	recursive.o \
	str_alg.o \
	utils.o \
	time_statistics.o \
	numbers.o \
	inversion.o \
	heap.o \
	number_trick.o \
	stack.o \
	#rsync.o \


CXXFLAGS=$(CFLAGS)

%.o : %.c
	$(CC) -c $(CFLAGS) -o $@ $<

%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

TARGET=alg

.PHONY:all
all : $(TARGET)
$(TARGET):$(LIB_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY:clean
clean:
	rm -rf $(TARGET) $(LIB_OBJS)
	rm -rf *.o	
