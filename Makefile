CC = gcc
AR = ar 
LD = ld

CFLAGS = -Wall -O2

INC = 
LIB = 

TARGET = libcshell.a

SRCDIRS=.
SRCS=$(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c))

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

%.o:%.c
	$(CC) -c $(INC) $(CFLAGS) $< -o $@

$(TARGET):$(OBJS)
	$(AR) -rc $(TARGET) $(OBJS)

.PHONY : clean
clean:
	@ rm -f *.o *.a
