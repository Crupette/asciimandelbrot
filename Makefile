SRCDIR		:= src
INCDIR		:= include
BINDIR		:= bin

OBJFILES	:= $(patsubst %.c,%.o,$(wildcard $(SRCDIR)/*.c))

CC		:= gcc
CFLAGS		:= -Wall -Werror -I$(INCDIR)

.PHONY: build run clean

build: $(OBJFILES) | $(BINDIR)
	$(CC) $(CFLAGS) -o $(BINDIR)/mandel $(OBJFILES)

run:
	./bin/mandel

clean:
	rm -f $(OBJFILES)

%.o : %.c 
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR):
	mkdir bin
