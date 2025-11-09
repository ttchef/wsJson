
CC = gcc
CFLAGS = -g -std=c99 -Iinclude/wsJson  -fsanitize=address -fsanitize=undefined
SHARED_CFLAGS = $(CFLAGS) -fPIC

BUILD ?= static

LIBNAME = wsJson

SRC_FILES = $(wildcard src/*.c)
OBJ_FILES = $(SRC_FILES:.c=.o)
SHARED_OBJ_FILES = $(SRC_FILES:.c=.shared.o)

all: $(BUILD)

shared: $(SHARED_OBJ_FILES)
	mkdir -p lib
	$(CC) $(SHARED_CFLAGS) -shared $(SHARED_OBJ_FILES) -o lib/lib$(LIBNAME).so

static: $(OBJ_FILES)
	mkdir -p lib
	ar cr lib/lib$(LIBNAME).a $(OBJ_FILES)

install: $(BUILD)
ifeq ($(BUILD),static)
		cp lib/lib$(LIBNAME).a /usr/local/lib/
else 
		cp lib/lib$(LIBNAME).so /usr/local/lib/
endif
		cp -r include/$(LIBNAME) /usr/local/include/

uninstall:
	rm -f /usr/local/lib/lib$(LIBNAME).a
	rm -f /usr/local/lib/lib$(LIBNAME).so
	rm -rf /usr/local/include/wsJson

example:
	$(CC) $(CFLAGS) example.c -o example -lwsJson


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.shared.o: %.c
	$(CC) $(SHARED_CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_FILES) $(SHARED_OBJ_FILES) lib$(LIBNAME).a lib$(LIBNAME).so lib example

