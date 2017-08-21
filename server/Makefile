CC = gcc
RM = -rm -f
# INCLUDES = -I../units
BUILDDIR = ../build
OUTDIR = ../output
TARGET = $(OUTDIR)/server

source = $(wildcard *.c)
object = $(patsubst %.c, $(BUILDDIR)/%.o, $(source))

all: $(TARGET)

$(BUILDDIR)/%.o: %.c
	# $(CC) -c $(INCLUDES) $< -o $@  
	$(CC) -c $< -o $@

$(TARGET): $(object)
	$(CC) -Wall -g -o $@ $(object)
.PHONY: default clean
clean:
	$(RM) $(TARGET) $(object)

install:
	sudo cp $(TARGET) /usr/local/bin
uninstall:
	sudo rm /usr/local/bin/server