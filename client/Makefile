CC = gcc
RM = -rm -f
# INCLUDE = -I ../units
BUILDDIR = ../build
OUTDIR = ../output
TARGET = $(OUTDIR)/client

source = $(wildcard *.c)
object = $(patsubst %.c, $(BUILDDIR)/%.o, $(source))

all: $(TARGET)

$(BUILDDIR)/%.o: %.c
	$(CC) -c $<  $(INCLUDE)  -o $@

$(TARGET): $(object)
	$(CC) -Wall -g -o $@ $(object)
.PHONY: default clean
clean:
	$(RM) $(TARGET) $(object)

install:
	sudo cp $(TARGET) /usr/local/bin
uninstall:
	sudo rm /usr/local/bin/client