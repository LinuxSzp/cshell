TARGET = cshell_demo

CC = gcc
CFLAGS = -Wall -O2 -g

INC = -I..

LIB = ../libcshell.a \
	  -lpthread

SRCDIRS=.
SRCS=$(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c))

OBJS=$(SRCS:.c=.o)

all:$(TARGET)

%.o:%.c
		$(CC) -c $(INC) $(CFLAGS) $< -o $@

$(TARGET):$(OBJS)
		$(CC) -o $@ $^ $(LIB) 

.PHONY : clean
clean:
		rm $(OBJS) $(TARGET) -f
