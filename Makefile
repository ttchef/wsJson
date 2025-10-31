
CC = gcc
CFLAGS = -o2

BUILD ?= static

LIBNAME = wsJson

all: $(BUILD)

shared: 
	$(CC) $(CFLAGS) -shared -fPIC ws_json.c -o lib$(LIBNAME).so

static: 
	ar cr lib$(LIBNAME).a ws_json.c

install: $(BUILD)
ifeq ($(BUILD),static)
		cp lib$(LIBNAME).a /usr/local/lib/
else 
		cp lib$(LIBNAME).so /usr/local/lib/
endif
		cp ws_json.h /usr/local/include/

uninstall:
	rm -f /usr/local/lib/lib$(LIBNAME).a
	rm -f /usr/local/lib/lib$(LIBNAME).so
	rm -f /usr/local/include/ws_json.h

clean:
	rm lib$(LIBNAME).a lib$(LIBNAME).so
