
CC = gcc
CFLAGS = -o2 -Iinclude/wsJson

BUILD ?= static

LIBNAME = wsJson

SRC_FILES = $(wildcard src/*.c)
OBJ_FILES = $(SRC_FILES:.c=.o)

all: $(BUILD)

shared: $(OBJ_FILES)
	mkdir -p lib
	$(CC) $(CFLAGS) -shared -fPIC $(OBJ_FILES) -o lib/lib$(LIBNAME).so

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
	rm -f /usr/local/include/

example: $(BUILD)
	$(CC) example.c -o example -lwsJson

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf lib$(LIBNAME).a lib$(LIBNAME).so lib

