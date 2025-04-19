CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

TARGET = shell

SRCDIR = src
INCDIR = include
SRCS = $(SRCDIR)/main.c $(SRCDIR)/init_pwd.c $(SRCDIR)/pwd.c $(SRCDIR)/exec_cmd.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)
	rm -f $(OBJS)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(SRCDIR)/%.o: $(SRCDIR)/%.c $(INCDIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)